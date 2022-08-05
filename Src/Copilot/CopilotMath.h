#pragma once
#include <math.h>

#include <Antilatency.Api.h>
#include <Antilatency.Math.h>

#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/mocap/mocap.h>

struct AnglesInDegrees {
    float rollDeg{};
    float pitchDeg{};
    float yawDeg{};
};

struct AnglesInRadians {
    float rollRad{};
    float pitchRad{};
    float yawRad{};
};

struct QuaternionDrone
{
    float w, x, y, z;
};

bool compare(float first,float second,float epsilon);

bool compareNEDandFloat3(mavsdk::Offboard::PositionNedYaw point,Antilatency::Math::float3 position,float epsilon);

mavsdk::Offboard::PositionNedYaw convertFloat3YawToNedYaw(Antilatency::Math::float3 position,float yawDegrees);

mavsdk::Mocap::VisionPositionEstimate convertTrackingPoseToVisionPositionEstimate(Antilatency::Math::floatP3Q pose);

// Convert degree to radians [-pi : pi]
float angleInDegToRaD(float angleInDeg);

// Convert Quaternion of Alt to Angle (degrees) of Drone PX4 (don't convert another Quaternion)
AnglesInDegrees quaternionAltToAngleDronePX4(Antilatency::Math::floatQ rotation);

// Convert Degrees of YAW ROLL PITCH to Radians
AnglesInRadians anglesInDegToRad(AnglesInDegrees anglesInDegrees);
