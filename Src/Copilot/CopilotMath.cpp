#include "CopilotMath.h"

bool compare(float first, float second, float epsilon){
    return (fabsf32(first-second)<=epsilon);
}

bool compareYaw(float first, float second, float epsilon)
{
    if((first > 270) && (second < 90)){
        second +=360;
    } else if((second > 270) && (first <90)){
        first += 360;
    }
    return compare(first,second,epsilon);
}

bool compareNEDandFloat3(mavsdk::Offboard::PositionNedYaw point, Antilatency::Math::float3 position, float epsilon){
    bool X = compare(point.east_m,position.x,epsilon);
    bool Z = compare(point.north_m,position.z,epsilon);
    bool Y = compare(-point.down_m,position.y,epsilon);
    if(X&&Y&&Z){
        return true;
    } else{
        return false;
    }
}

mavsdk::Offboard::PositionNedYaw convertFloat3YawToNedYaw(Antilatency::Math::float3 position, float yawDegrees){
    mavsdk::Offboard::PositionNedYaw pointNED;
    pointNED.north_m = position.z;
    pointNED.east_m = position.x;
    pointNED.down_m = -position.y;
    pointNED.yaw_deg = yawDegrees;
    return pointNED;
}

float angleInDegToRaD(float inDeg){
    float angleInRad = 0;
    if(inDeg>=-180 && inDeg<=180){
        angleInRad = inDeg/57.2957801;
    } else if(inDeg>180 && inDeg<=360){
        angleInRad = (inDeg-360)/57.2957801;
    } else if(inDeg>360){
        angleInRad = angleInDegToRaD(inDeg-360);
    } else if(inDeg < -180){
        angleInRad = angleInDegToRaD(inDeg+180);
    }
    return angleInRad;
}

AnglesInDegrees quaternionAltToAngleDronePX4(Antilatency::Math::floatQ rotation){
    float yaw{};
    float x = rotation.x;
    float y = rotation.y;
    float z = rotation.z;
    float w = rotation.w;
    AnglesInDegrees angleInDegrees;
    angleInDegrees.rollDeg = atan2(2.0*(w*z+x*y),1-2.0*(z*z+x*x))*57.2957801;
    angleInDegrees.pitchDeg = asin(2.0*(x*w-y*z))*57.2957801;
    yaw = atan2(2.0*(y*w+z*x),(-1+2*(w*w+z*z)));
    if(yaw>=0){
        yaw=yaw*57.2957801;
    }
    if(yaw<0) {
        yaw=360+yaw*57.2957801;
    }
    if(yaw>=360){
        yaw = yaw - 360;
    }
    angleInDegrees.yawDeg = yaw;
    return angleInDegrees;
}

AnglesInRadians anglesInDegToRad(AnglesInDegrees anglesInDegrees){
    AnglesInRadians anglesInRadians;
    anglesInRadians.yawRad = angleInDegToRaD(anglesInDegrees.yawDeg);
    anglesInRadians.rollRad = angleInDegToRaD(anglesInDegrees.rollDeg);
    anglesInRadians.pitchRad = angleInDegToRaD(anglesInDegrees.pitchDeg);
    return anglesInRadians;
}

mavsdk::Mocap::VisionPositionEstimate convertTrackingPoseToVisionPositionEstimate(Antilatency::Math::floatP3Q pose)
{
    mavsdk::Mocap::VisionPositionEstimate trackingDataForPX4;
    std::vector<float> matrix = {NAN};

    AnglesInDegrees angleDegreesFromAlt;
    AnglesInRadians angleInRadians;

    angleDegreesFromAlt = quaternionAltToAngleDronePX4(pose.rotation);
    angleInRadians = anglesInDegToRad(angleDegreesFromAlt);
    trackingDataForPX4.position_body.x_m = pose.position.z;
    trackingDataForPX4.position_body.y_m = pose.position.x;
    trackingDataForPX4.position_body.z_m = -pose.position.y;

    trackingDataForPX4.angle_body.yaw_rad = angleInRadians.yawRad;
    trackingDataForPX4.angle_body.roll_rad = angleInRadians.rollRad;
    trackingDataForPX4.angle_body.pitch_rad = angleInRadians.pitchRad;
    trackingDataForPX4.pose_covariance.covariance_matrix = matrix;

    return trackingDataForPX4;
}
