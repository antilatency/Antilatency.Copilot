#include "FeederPX4.h"

FeederPX4::FeederPX4(PX4 &px4, TrackingForCopilot &tracking){
    _px4 = &px4;
    _tracking = &tracking;
}

bool FeederPX4::feedPX4DataFromTracking(){
    mavsdk::Mocap::VisionPositionEstimate trackingDataForPX4;
    std::vector<float> matrix = {NAN};

    AnglesInDegrees angleDegreesFromAlt;
    AnglesInRadians angleInRadians;
    Antilatency::Alt::Tracking::State state;
    float correctionYaw = 0.0;

    while(true){
        if(_px4->isConnected() && _tracking->isStateReady()){
            state = _tracking->getState();
            angleDegreesFromAlt = quaternionAltToAngleDronePX4(state.pose.rotation,correctionYaw);
            angleInRadians = anglesInDegToRad(angleDegreesFromAlt);
            trackingDataForPX4.position_body.x_m = state.pose.position.z;
            trackingDataForPX4.position_body.y_m = state.pose.position.x;
            trackingDataForPX4.position_body.z_m = -state.pose.position.y;

            trackingDataForPX4.angle_body.yaw_rad = angleInRadians.yawRad;
            trackingDataForPX4.angle_body.roll_rad = angleInRadians.rollRad;
            trackingDataForPX4.angle_body.pitch_rad = angleInRadians.pitchRad;
            trackingDataForPX4.pose_covariance.covariance_matrix = matrix;

            _px4->setTrackerData(trackingDataForPX4);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return true;
}
