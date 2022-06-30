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

    bool arm_();
    bool disarm_();
    bool land_();
    bool kill_();
    bool offboardStart_();

    bool goToPoint(mavsdk::Offboard::PositionNedYaw target);
    bool stopMoving(mavsdk::Offboard::PositionNedYaw target);
    bool setVelocityNED(mavsdk::Offboard::VelocityNedYaw velocity);

    void setVisionPositionEstimate();
    
private:

    void static getConnection(mavsdk::Mavsdk& mavsdk);
    std::shared_ptr<mavsdk::System> static getSystem(mavsdk::Mavsdk& mavsdk);
    bool offboardResult(mavsdk::Offboard::Result& result);

    mavsdk::Mavsdk _mavsdk;
    std::shared_ptr<mavsdk::System> _system;
    mavsdk::Action* _action;
    mavsdk::Offboard* _offboard;
    mavsdk::Telemetry* _telemetry;
    mavsdk::Mocap* _mocap;

    mavsdk::Telemetry::PositionNed _telemetryDataNED;
    mavsdk::Mocap::VisionPositionEstimate _trackerData;

    bool _telemetryReady=false;
    bool _systemAndPluginsReady = false;
    bool _trackerDataReady = false;

};
