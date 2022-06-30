#pragma once
#include <iostream>
#include <chrono>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugin_base.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mocap/mocap.h>
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>
#include <mavsdk/plugins/param/param.h>
#include <mavsdk/geometry.h>
#include <mavsdk/plugins/tune/tune.h>

#include <Antilatency.Math.h>

#include "CopilotMath.h"
#include "PX4.h"
#include "TrackingForCopilot.h"

enum StageTargetLanding {
    GetingTargetLand,
    GoToSafeAltitude,
    MovingToSafeAltitude,
    GoToTargetOnSafeAltitude,
    MovingToTargetOnSafeAltitude,
    DescentUnderTarget,
    WaitingConditionForLanding,
    PressInTarget,
    RescueProtocol,
    FinishTargetLanding,
    DetecterFaildOfLanding
};

class TargetLanding {
public:
    TargetLanding(PX4& px4, TrackingForCopilot& tracking);

    void executingTargetLanding(float x,
                           float z,
                           float y,
                           float yaw,
                           float safeAltitude,
                           float precisionYaw,
                           float precisionPose,
                           bool &notMyLandCommand,
                           bool &takeOverAndLand);

private:

        PX4* _px4;
        TrackingForCopilot* _tracking;

        float _safeAltitude;
        mavsdk::Offboard::PositionNedYaw _pointOfMove;
        mavsdk::Offboard::PositionNedYaw _landTarget;

        float _stepDown = 0.12;
        float _precisionForWayPoint = 0.075;

        StageTargetLanding _stageTargetLanding = GetingTargetLand;
        int _countRescueSituation = 0;
        long long _timeOfStartPressin;
        long long _timeNow;
};
