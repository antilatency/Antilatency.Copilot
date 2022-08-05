#pragma once
#include <future>
#include <optional>

#include "Application.h"
#include "CopilotMath.h"

#include <Antilatency.Api.h>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugin_base.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mocap/mocap.h>
#include <mavsdk/plugins/offboard/offboard.h>

class TargetLandingApplication : public Application
{
    const std::string typeConnection        = "serial://";
    const std::string path                  = "/dev/serial0";
    const std::string baudrate              = ":921600";

    const int   periodOfFeedPX4TrackingPoseMs = 20; // 50 Hz

    const float precisionForWayPoint        = 0.030;
    const float altitudeAboveTargetForLand  = 0.025;
    const float stepDown                    = 0.100;
    const float fallDetectionThreshold      = 0.010;
    const int   maxFaildOfLanding           = 3;
    const int   timeToConfirmLandSec        = 3;

public:
    struct LandingTargetProperties{
        Antilatency::Math::float3 position;
        float yaw;
        float safeAltitude;
        float precisionForPose;
        float precisionForYaw;
    };

    TargetLandingApplication(LandingTargetProperties landingTargetProperties, Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary,
                        Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary,
                        Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary,
                        std::string environmentData, std::string placementData);

    int run() override;

private:
    void doTargetLanding(std::optional<Antilatency::Alt::Tracking::State> trackingStage);
    std::shared_ptr<mavsdk::System> getSystem();

    mavsdk::Mavsdk _mavsdk;
    std::shared_ptr<mavsdk::System> _system;
    std::shared_ptr<mavsdk::Mocap> _mocap;
    std::shared_ptr<mavsdk::Telemetry> _telemetry;
    std::shared_ptr<mavsdk::Offboard> _offboard;
    std::shared_ptr<mavsdk::Action> _action;

    enum StageOfLanding {
        WaitingForLandCommand,
        StartMoveToSafeAltitude,
        MovingToSafeAltitude,
        StartMoveToTargetOnSafeAltitude,
        MovingToTargetOnSafeAltitude,
        DescentAboveTarget,
        WaitingStateForLanding,
        PressInTarget,
        RescueProtocol,
        FinishLanding,
        DetecterFaildOfLanding
    };

    StageOfLanding _stageTargetLanding = WaitingForLandCommand;
    LandingTargetProperties _landingTargetProperties;
    int _countFaildOfLanding = 0;
    std::chrono::system_clock::time_point _stagePressInTargetStartTime{};
};
