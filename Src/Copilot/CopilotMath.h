#pragma once
#include <math.h>

#include <Antilatency.Api.h>
#include <Antilatency.Math.h>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugin_base.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

bool equal(float first,float second,float epsilon);

bool equalNEDandFloat3(mavsdk::Offboard::PositionNedYaw point,Antilatency::Math::float3 position,float epsilon);

mavsdk::Offboard::PositionNedYaw convertFloat3ToNED(Antilatency::Math::float3 position,float yawDegrees);
