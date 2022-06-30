#include "CopilotMath.h"

bool equal(float first, float second, float epsilon){
    return (fabsf32(first-second)<=epsilon);
}

bool equalNEDandFloat3(mavsdk::Offboard::PositionNedYaw point, Antilatency::Math::float3 position, float epsilon){
    bool X = equal(point.east_m,position.x,epsilon);
    bool Z = equal(point.north_m,position.z,epsilon);
    bool Y = equal(-point.down_m,position.y,epsilon);
    if(X&&Y&&Z){
        return true;
    } else{
        return false;
    }
}

mavsdk::Offboard::PositionNedYaw convertFloat3ToNED(Antilatency::Math::float3 position, float yawDegrees){
    mavsdk::Offboard::PositionNedYaw pointNED;
    pointNED.north_m = position.z;
    pointNED.east_m = position.x;
    pointNED.down_m = -position.y;
    pointNED.yaw_deg = yawDegrees;
    return pointNED;
}
