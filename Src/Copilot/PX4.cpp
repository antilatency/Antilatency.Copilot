#include "PX4.h"

using std::chrono::seconds;

void PX4::connectToPX4(){
    getConnection(_mavsdk);
    _system = getSystem(_mavsdk);
}

void PX4::setVisionPositionEstimate()
{
    while(true){
        if(_trackerDataReady){
            _mocap->set_vision_position_estimate(_trackerData);
            _trackerDataReady = false;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void PX4::createPluginsPX4(){

    std::cout   <<  "      PX4:"        <<  std::endl;
    std::cout   <<  "Create plugins:"   <<  std::endl;

    std::cout   <<  "\"Telemetry\""     <<  std::endl;
    auto telemetry = mavsdk::Telemetry{_system};
    _telemetry = &telemetry;

    std::cout   <<  "\"Mocap\""         <<  std::endl;
    auto mocap = mavsdk::Mocap{_system};
    _mocap = &mocap;

    std::cout   <<  "\"Action\""        <<  std::endl;
    auto action = mavsdk::Action{_system};
    _action = &action;

    std::cout   <<  "\"Offboard\""      <<  std::endl;
    auto offboard = mavsdk::Offboard{_system};
    _offboard = &offboard;

    _systemAndPluginsReady = true;

    setVisionPositionEstimate();
}

bool PX4::isConnected(){
    if(_systemAndPluginsReady){
        return _system->is_connected();
    } else {
        return false;
    }
}

bool PX4::isArming(){
    return _telemetry->armed();
}

bool PX4::isLanding(){
    return (mavsdk::Telemetry::FlightMode::Land == _telemetry->flight_mode());
}

bool PX4::isOffboard()
{
    return (mavsdk::Telemetry::FlightMode::Offboard == _telemetry->flight_mode());
}

mavsdk::Telemetry::PositionNed PX4::getTelemetryDataNED(){
    return _telemetryDataNED;
}

void PX4::setTrackerData(mavsdk::Mocap::VisionPositionEstimate data){
    _trackerData = data;
    _trackerDataReady = true;
}

bool PX4::arm_(){
    if(!(this->isConnected())){
        std::cout << "Arming faild - lost system" << std::endl;
        return false;
    }

    mavsdk::Action::Result result = _action->arm();
    if (result == mavsdk::Action::Result::Success){
        return true;
    } else {
        std::cout << "Arming:\t" << result << std::endl;
        return false;
    }
}

bool PX4::disarm_(){
    if(!(this->isConnected())){
        std::cout << "Disarming faild - lost system" << std::endl;
        return false;
    }

    const mavsdk::Action::Result result = _action->disarm();
    if (result == mavsdk::Action::Result::Success){
        return true;
    } else {
        std::cout << "Disarming: " << result << std::endl;
        return false;
    }
}

bool PX4::land_(){
    if(!(this->isConnected())){
        std::cout << "Land faild - lost system" << std::endl;
        return false;
    }

    const mavsdk::Action::Result result = _action->land();
    if (result == mavsdk::Action::Result::Success){
        return true;
    } else {
        std::cout << "Landing: " << result << std::endl;
        return false;
    }
}

bool PX4::kill_(){
    if(!(this->isConnected())){
        std::cout << "Kill faild - lost system" << std::endl;
        return false;
    }

    const mavsdk::Action::Result result = _action->kill();
    if (result == mavsdk::Action::Result::Success){
        return true;
    } else {
        std::cout << "Kill: " << result << std::endl;
        return false;
    }
}

bool PX4::offboardStart_(){
    if(!(this->isConnected())){
        std::cout << "Offboard faild - lost system" << std::endl;
        return false;
    }

    mavsdk::Offboard::VelocityBodyYawspeed stay{0,0,0,0};
    _offboard->set_velocity_body(stay);

    mavsdk::Offboard::Result result = _offboard->start();
    std::cout << "Start offboard:\t" << std::endl;
    return offboardResult(result);
}

bool PX4::goToPoint(mavsdk::Offboard::PositionNedYaw target){
    mavsdk::Offboard::Result result = _offboard->set_position_ned(target);
    std::cout << "Go to point:\t" << std::endl;
    return offboardResult(result);
}

bool PX4::stopMoving(mavsdk::Offboard::PositionNedYaw target){
    mavsdk::Offboard::Result result = _offboard->set_velocity_body({0,0,0,0});
    if(!offboardResult(result)){
        return false;
    }
    result = _offboard->set_position_ned(target);
    return offboardResult(result);
}

bool PX4::setVelocityNED(mavsdk::Offboard::VelocityNedYaw velocity){
    mavsdk::Offboard::Result offboard_result = _offboard->set_velocity_ned(velocity);
    return offboardResult(offboard_result);
}

float PX4::getYaw(){
    if(_systemAndPluginsReady){
        if(_system->is_connected()){
            return _telemetry->attitude_euler().yaw_deg;
        } else {
            return NAN;
        }
    } else {
        return NAN;
    }
}

void PX4::getConnection(mavsdk::Mavsdk &mavsdk){
    mavsdk::ConnectionResult connection_result = mavsdk.add_any_connection("serial:///dev/serial0:921600");
    if (connection_result != mavsdk::ConnectionResult::Success) {
        std::cerr << connection_result << std::endl;
    } else {
        std::cout << connection_result << std::endl;
    }
}

std::shared_ptr<mavsdk::System> PX4::getSystem(mavsdk::Mavsdk &mavsdk) //Discoverd Autopilot
{
    std::cout << "Waiting to discover system...\n";
    auto prom = std::promise<std::shared_ptr<mavsdk::System>>{};
    auto fut = prom.get_future();

    // We wait for new systems to be discovered, once we find one that has an
    // autopilot, we decide to use it.
    mavsdk.subscribe_on_new_system([&]() {
        auto system = mavsdk.systems().back();

        if (system->has_autopilot()) {
            std::cout << "Discovered autopilot\n";

            // Unsubscribe again as we only want to find one system.
            mavsdk.subscribe_on_new_system(nullptr);
            prom.set_value(system);
        }
    });

    // We usually receive heartbeats at 1Hz, therefore we should find a
    // system after around 3 seconds max, surely.
    while(true){
        if (fut.wait_for(seconds(3)) == std::future_status::timeout) {
            std::cerr << "No autopilot found.\n";
        }   else {
            break;
        }
    }
    // Get discovered system now.
    return fut.get();
}

bool PX4::offboardResult(mavsdk::Offboard::Result &result)
{
    if (result == mavsdk::Offboard::Result::Success){
        return true;
    } else {
        return false;
    }
}
