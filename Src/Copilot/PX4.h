#pragma once

#include <iostream>
#include <future>

#include <mavsdk/mavsdk.h>
#include <mavsdk/plugin_base.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavsdk/plugins/mocap/mocap.h>
#include <mavsdk/plugins/offboard/offboard.h>

class PX4 {
public:
    void connectToPX4();
    void createPluginsPX4();

    bool isConnected();
    bool isArming();
    bool isLanding();
    bool isOffboard();

    mavsdk::Telemetry::PositionNed getTelemetryDataNED();
    void setTrackerData(mavsdk::Mocap::VisionPositionEstimate data);
    float getYaw();

    bool doArm();
    bool doDisarm();
    bool doLand();
    bool doKill();
    bool offboardStart();

    bool goToPoint(mavsdk::Offboard::PositionNedYaw target);
    bool stopMoving(mavsdk::Offboard::PositionNedYaw target);
    bool setVelocityNED(mavsdk::Offboard::VelocityNedYaw velocity);

    void setVisionPositionEstimate();
    
private:

    void static addConnection(mavsdk::Mavsdk& mavsdk);
    std::shared_ptr<mavsdk::System> static getSystem(mavsdk::Mavsdk& mavsdk);
    bool offboardResult(mavsdk::Offboard::Result& result);

    mavsdk::Mavsdk _mavsdk;
    std::shared_ptr<mavsdk::System> _system;
    mavsdk::Action* _action;
    mavsdk::Offboard* _offboard;
    mavsdk::Telemetry* _telemetry;
    mavsdk::Mocap* _mocap;

    mavsdk::Telemetry::PositionNed _telemetryDataNED;


    bool _telemetryReady=false;
    bool _systemAndPluginsReady = false;
    bool _trackerDataReady = false;
    mavsdk::Mocap::VisionPositionEstimate _trackerData;

};
