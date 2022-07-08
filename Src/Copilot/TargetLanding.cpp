#include "TargetLanding.h"

TargetLanding::TargetLanding(PX4 &px4, TrackingForCopilot &tracking){

    _px4 = &px4;
    _tracking = &tracking;
}

void TargetLanding::executingTargetLanding(float x, float z, float y, float yaw,
                                 float safeAltitude, float precisionYaw, float precisionPose,
                                 bool &waitForLandingCommand, bool &takeOverAndLand)
{

    float _precisionForWayPoint = 0.075;

    auto positionTracking = _tracking->getPosition();
    auto yawTracking = _tracking->getYaw();

        if(_px4->isLanding() && _px4->isArming()){
            std::cout << "I recived Land command again!" << std::endl;
            _px4->offboardStart_();
            _stageTargetLanding = GetingTargetLand;
        }

        if(_stageTargetLanding == GetingTargetLand){
            _landTarget = {z,x,-y,yaw};
            _safeAltitude = -safeAltitude;
            std::cout << "Safe Altitude:\t"<<_safeAltitude << std::endl;
            std::cout << "Starting target Land" << std::endl;
            _stageTargetLanding = GoToSafeAltitude;
        }

        if(_stageTargetLanding == GoToSafeAltitude){
            _pointOfMove = convertFloat3ToNED(positionTracking,yawTracking);
            _pointOfMove.down_m = _safeAltitude;
            _px4->goToPoint(_pointOfMove);
            std::cout << "Moving to safe altitude" << std::endl;
            _stageTargetLanding = MovingToSafeAltitude;
        }

        if(_stageTargetLanding == MovingToSafeAltitude){
            if(equal((-_safeAltitude),positionTracking.y,_precisionForWayPoint)){
                std::cout << "Arrived to safe altitude" << std::endl;
                _stageTargetLanding = GoToTargetOnSafeAltitude;
            }
        }

        if(_stageTargetLanding == GoToTargetOnSafeAltitude){
            _pointOfMove = _landTarget;
            _pointOfMove.down_m = _safeAltitude;
            _px4->goToPoint(_pointOfMove);
            std::cout << "Moving to target on safe altitude" << std::endl;
            _stageTargetLanding = MovingToTargetOnSafeAltitude;
        }

        if(_stageTargetLanding == MovingToTargetOnSafeAltitude){
            if(equal(_landTarget.north_m,positionTracking.z,_precisionForWayPoint) && equal(_landTarget.east_m,positionTracking.x,_precisionForWayPoint)){
                _stageTargetLanding = DescentUnderTarget;
            } else {
                _stageTargetLanding = MovingToTargetOnSafeAltitude;
            }
        }

        if(_stageTargetLanding == DescentUnderTarget){
            float descentAltitude = _landTarget.down_m-0.10;
            float conditionAltitude = 0.05;
            _pointOfMove = _landTarget;

            if((positionTracking.y)<=(-descentAltitude)){
                _pointOfMove.down_m -= conditionAltitude;
                _px4->goToPoint(_pointOfMove);
                std::cout << "Waiting condition for land" << std::endl;
                _stageTargetLanding = WaitingConditionForLanding;
            } else{
                float multiplier = 1.0;
                if(_tracking->getPosition().y>(-_landTarget.down_m+0.3)){
                    multiplier = _tracking->getPosition().y/(-_landTarget.down_m+0.3);
                } else{
                    multiplier = 1.0;
                }
                _pointOfMove.down_m = (-_tracking->getPosition().y)+_stepDown*multiplier;
                _px4->goToPoint(_pointOfMove);
            }
        }

        if(_stageTargetLanding == WaitingConditionForLanding){
            if(equal(positionTracking.z,_landTarget.north_m,precisionPose)
                    &&equal(positionTracking.x,_landTarget.east_m,precisionPose)
                    &&equal(yawTracking,_landTarget.yaw_deg,precisionYaw)){
                _pointOfMove.east_m =positionTracking.x;
                _pointOfMove.north_m = positionTracking.z;
                _pointOfMove.down_m = (-positionTracking.y)+_stepDown;
                _px4->goToPoint(_pointOfMove);
                std::cout << "Press in started." << std::endl;
                _stageTargetLanding = PressInTarget;
                _timeOfStartPressin = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            } else {
                _stageTargetLanding = WaitingConditionForLanding;
            }
        }

        if(_stageTargetLanding == PressInTarget){
            //Timeout if all ok
            _timeNow = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
            if((_timeNow-_timeOfStartPressin)>=3){
                std::cout << "Position of landing target confirmed" << std::endl;
                _timeOfStartPressin = 0;
                _timeNow = 0;
                _stageTargetLanding = FinishTargetLanding;
            } else {
               _pointOfMove.east_m =positionTracking.x;
               _pointOfMove.north_m = positionTracking.z;
               _pointOfMove.down_m = (-positionTracking.y)+_stepDown;
               _px4->goToPoint(_pointOfMove);
               _stageTargetLanding = DetecterFaildOfLanding;
            }
        }

        if(_stageTargetLanding == DetecterFaildOfLanding){
            //Detect Fall
            if(((-_landTarget.down_m)-positionTracking.y)>0.01){
                std::cout << "Detected Fall" << std::endl;
                _stageTargetLanding = RescueProtocol;
            }
            //Detect faild position;
            bool faildYAW   = !equal(yawTracking,_landTarget.yaw_deg,precisionYaw);
            bool faildZ     = !equal(positionTracking.z,_landTarget.north_m,precisionPose);
            bool faildX     = !equal(positionTracking.x,_landTarget.east_m,precisionPose);
            if(faildYAW || faildZ || faildX){
                std::cout << "Detected faild position\n"
                              << "Yaw:\t"   << faildYAW << "\n"
                              << "Z:\t"     << faildZ   << "\n"
                              << "X:\t"     << faildX   << std::endl;
                _stageTargetLanding = RescueProtocol;
            } else {
                _stageTargetLanding = PressInTarget;
            }
        }

        if(_stageTargetLanding == RescueProtocol){
            _countRescueSituation++;
            _pointOfMove = convertFloat3ToNED(positionTracking,yawTracking);
            _pointOfMove.down_m = _safeAltitude;
            _px4->goToPoint(_pointOfMove);
            std::cout << "Moving to safe altitude" << std::endl;
            if(_countRescueSituation<=3){
                _stageTargetLanding = MovingToSafeAltitude;
            } else {
                std::cout<< "Can't to landing at this target! Please take over manual control" << std::endl;
                takeOverAndLand = false;
                waitForLandingCommand = false;
            }
        }

        if(_stageTargetLanding == FinishTargetLanding){
            _px4->kill_();
            if(!_px4->isArming()){
                _countRescueSituation = 0;
                _stageTargetLanding = GetingTargetLand;
                std::cout << "Land!" << std::endl;
                waitForLandingCommand = true;
                takeOverAndLand = false;
            }
        }
}
