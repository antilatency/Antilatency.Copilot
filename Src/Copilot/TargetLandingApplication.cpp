#include "TargetLandingApplication.h"

TargetLandingApplication::TargetLandingApplication(TargetLandingApplication::LandingTargetProperties landingTargetProperties, Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary, Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary, Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary, std::string environmentData, std::string placementData)
    : Application(deviceNetworkLibrary, altTrackingLibrary, environmentSelectorLibrary, environmentData, placementData),
    _landingTargetProperties{landingTargetProperties}
{
    std::string connection = typeConnection + path + baudrate;
    mavsdk::ConnectionResult connection_result = _mavsdk.add_any_connection(connection);
    if(connection_result != mavsdk::ConnectionResult::Success) {
        throw std::runtime_error("Failed to add any connection mavsdk");
    }
    _system     = getSystem();
    _mocap      = std::make_shared<mavsdk::Mocap>(_system);
    _telemetry  = std::make_shared<mavsdk::Telemetry>(_system);
    _offboard   = std::make_shared<mavsdk::Offboard>(_system);
    _action     = std::make_shared<mavsdk::Action>(_system);
    _mavlinkPassthrough = std::make_shared<mavsdk::MavlinkPassthrough>(_system);
}

void TargetLandingApplication::run()
{
    auto timeBegin =  std::chrono::system_clock::now();

    _mavlinkPassthrough->subscribe_message_async(
                MAVLINK_MSG_ID_HEARTBEAT, [&](const mavlink_message_t& message) {
        mavlink_heartbeat_t heartbeat;
        mavlink_msg_heartbeat_decode(&message, &heartbeat);

        switch (heartbeat.autopilot) {
        case Ardupilot:{
            _typeOfAutopilot = Ardupilot;
        } break;
        case PX4:{
            _typeOfAutopilot = PX4;
        } break;
        default:{
            _typeOfAutopilot = Unknown;
        }
        }

        if(_typeOfAutopilot == Ardupilot){
            if(heartbeat.custom_mode == ActionForArdupilot::FlightModeOfArdupilot::Guided){
                _flightMode = mavsdk::Telemetry::FlightMode::Offboard;
            } else {
                _flightMode = _telemetry->flight_mode();
            }
        } else {
            _flightMode = _telemetry->flight_mode();
        }
    });

    while(!_typeOfAutopilot.has_value()){
        std::this_thread::yield();
    }

    switch(_typeOfAutopilot.value()){
    case PX4:{
        _copilotAction = new ActionForPX4(_mavlinkPassthrough,_offboard);
    } break;
    case Ardupilot:{
        _copilotAction = new ActionForArdupilot(_mavlinkPassthrough,_offboard);
    } break;
    case Unknown:{
        _mavlinkPassthrough->subscribe_message_async(MAVLINK_MSG_ID_HEARTBEAT,nullptr);
        throw std::runtime_error("Unknown type of autopilot");
    }break;
    }

    while(true){
        auto trackingState = getTrackingState();
        //Feed autopilot with tracking data
        if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now()-timeBegin).count() >= periodOfFeedPX4TrackingPoseMs){
            timeBegin = std::chrono::system_clock::now();
            if(trackingState.has_value() && (trackingState->stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof)){
                _mocap->set_vision_position_estimate(convertTrackingPoseToVisionPositionEstimate(trackingState->pose));
            }
        }

        doTargetLanding(trackingState);

        std::this_thread::yield();
    }
}


void TargetLandingApplication::doTargetLanding(std::optional<Antilatency::Alt::Tracking::State> trackingState){

    //Stop Offboard mode if tracking lost
    if((!trackingState.has_value()) || (trackingState->stability.stage != Antilatency::Alt::Tracking::Stage::Tracking6Dof)){
        if(_flightMode == mavsdk::Telemetry::FlightMode::Offboard){
            _copilotAction->giveAwayControl();
        }
        _stageTargetLanding = WaitingForLandCommand;
        return;
    }

     //Take control when Landing starts
    if((_flightMode == mavsdk::Telemetry::FlightMode::Land) && _telemetry->armed()){
        _copilotAction->takeOverControl();
        _stageTargetLanding = StartMoveToSafeAltitude;
    }
    //if was taked manual control
    if((_stageTargetLanding != WaitingForLandCommand) && (_flightMode != mavsdk::Telemetry::FlightMode::Offboard) && (_flightMode != mavsdk::Telemetry::FlightMode::Land)){
        _stageTargetLanding = WaitingForLandCommand;
        return;
    }

    auto yawTracking = quaternionAltToAngleDronePX4(trackingState->pose.rotation).yawDeg;
    auto positionTrackingNED = convertFloat3YawToNedYaw(trackingState->pose.position, yawTracking);
    auto safeAltitude = -_landingTargetProperties.safeAltitude;
    mavsdk::Offboard::PositionNedYaw pointOfMove;
    const mavsdk::Offboard::PositionNedYaw landTarget = {_landingTargetProperties.position.z, _landingTargetProperties.position.x, -_landingTargetProperties.position.y, _landingTargetProperties.yaw};

    switch(_stageTargetLanding){
    case WaitingForLandCommand:
        break;
    case StartMoveToSafeAltitude:{
        pointOfMove = positionTrackingNED;
        pointOfMove.down_m = safeAltitude;
        _offboard->set_position_ned(pointOfMove);
        _stageTargetLanding = MovingToSafeAltitude;
    } break;
    case MovingToSafeAltitude:{
        if(compare(safeAltitude, positionTrackingNED.down_m, precisionForWayPoint)){
            _stageTargetLanding = StartMoveToTargetOnSafeAltitude;
        }
    } break;
    case StartMoveToTargetOnSafeAltitude:{
        pointOfMove = landTarget;
        pointOfMove.down_m = safeAltitude;
        _offboard->set_position_ned(pointOfMove);
        _stageTargetLanding = MovingToTargetOnSafeAltitude;
    } break;
    case MovingToTargetOnSafeAltitude:{
        if(compare(landTarget.north_m, positionTrackingNED.north_m, precisionForWayPoint) &&
                compare(landTarget.east_m, positionTrackingNED.east_m, precisionForWayPoint)){
            _stageTargetLanding = DescentAboveTarget;
        }
    } break;
    case DescentAboveTarget:{
        float descentAltitude = landTarget.down_m - altitudeAboveTargetForLand*2;
        pointOfMove = landTarget;
        if(positionTrackingNED.down_m >= descentAltitude){
            pointOfMove.down_m = landTarget.down_m - altitudeAboveTargetForLand;
            _offboard->set_position_ned(pointOfMove);
            _stageTargetLanding = WaitingStateForLanding;
        } else {
            pointOfMove.down_m = positionTrackingNED.down_m + stepDown;
            _offboard->set_position_ned(pointOfMove);
        }
    } break;
    case WaitingStateForLanding:{
        if(compare(positionTrackingNED.north_m, landTarget.north_m, _landingTargetProperties.precisionForPose)
                &&compare(positionTrackingNED.east_m, landTarget.east_m, _landingTargetProperties.precisionForPose)
                &&compareYaw(positionTrackingNED.yaw_deg, landTarget.yaw_deg, _landingTargetProperties.precisionForYaw)){
            pointOfMove = landTarget;
            pointOfMove.down_m += stepDown;
            _offboard->set_position_ned(pointOfMove);
            _stagePressInTargetStartTime = std::chrono::system_clock::now();
            _stageTargetLanding = PressInTarget;
        }
    } break;
    case PressInTarget:{
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-_stagePressInTargetStartTime).count() >= timeToConfirmLandSec){
            _stageTargetLanding = FinishLanding;
        } else {
            pointOfMove = positionTrackingNED;
            pointOfMove.down_m += stepDown;
            _offboard->set_position_ned(pointOfMove);
            _stageTargetLanding = DetecterFaildOfLanding;
        }
    } break;
    case DetecterFaildOfLanding:{
        if(((-landTarget.down_m) - (-positionTrackingNED.down_m)) > fallDetectionThreshold){
            std::cout << "Detected fall" << std::endl;
            _stageTargetLanding = RescueProtocol;
        }
        bool errorYAW   = !compareYaw(positionTrackingNED.yaw_deg, landTarget.yaw_deg, _landingTargetProperties.precisionForYaw);
        bool errorNorth = !compare(positionTrackingNED.north_m, landTarget.north_m, _landingTargetProperties.precisionForPose);
        bool errorEast  = !compare(positionTrackingNED.east_m, landTarget.east_m, _landingTargetProperties.precisionForPose);
        if(errorYAW || errorNorth || errorEast){
            std::cout << "Detected error position!" << std::endl;
            _stageTargetLanding = RescueProtocol;
        } else {
            _stageTargetLanding = PressInTarget;
        }
    } break;
    case RescueProtocol:{
        pointOfMove = landTarget;
        pointOfMove.down_m = safeAltitude;
        _offboard->set_position_ned(pointOfMove);
        if(_countFaildOfLanding >= maxFaildOfLanding){
            std::cout<< "Can't land on this target! Please take over manual control" << std::endl;
            _countFaildOfLanding = 0;
            _stageTargetLanding = WaitingForLandCommand;
        } else {
            _stageTargetLanding = MovingToSafeAltitude;
        }
        _countFaildOfLanding++;
    } break;
    case FinishLanding:{
        _action->kill();
        if(!_telemetry->armed()){
            _countFaildOfLanding = 0;
            _stageTargetLanding = WaitingForLandCommand;
            std::cout << "Land!" << std::endl;
        }
    } break;
    }
}

std::shared_ptr<mavsdk::System> TargetLandingApplication::getSystem(){
    auto prom = std::promise<std::shared_ptr<mavsdk::System>>{};
    auto fut = prom.get_future();

    _mavsdk.subscribe_on_new_system([&]() {
        auto system = _mavsdk.systems().back();

        if (system->has_autopilot()) {
            _mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        }
    });

    while(true){
        if (fut.wait_for(std::chrono::seconds(3)) == std::future_status::timeout) {
            std::cerr << "No autopilot found." << std::endl;
        }   else {
            break;
        }
    }

    return fut.get();
}
