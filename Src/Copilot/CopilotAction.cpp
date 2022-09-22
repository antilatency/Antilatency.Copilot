#include "CopilotAction.h"

CopilotAction::CopilotAction(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough, std::shared_ptr<mavsdk::Offboard> offboard):
    _mavlinkPassthrough{mavlinkPassthrough},
    _offboard{offboard}
{

}

ActionForArdupilot::ActionForArdupilot(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough, std::shared_ptr<mavsdk::Offboard> offboard)
    :CopilotAction(mavlinkPassthrough,offboard)
{

}

void ActionForArdupilot::takeOverControl(){
    setGuidedFlightModeToArdupilot();
    _offboard->set_velocity_body({0,0,0,0});
}

void ActionForArdupilot::giveAwayControl(){
    _offboard->set_velocity_body({0,0,0,0});
    setStabilizeFlightModeToArdupilot();
}

void ActionForArdupilot::setStabilizeFlightModeToArdupilot(){
    mavsdk::MavlinkPassthrough::CommandLong commandLong;
    commandLong.command         = MAV_CMD_DO_SET_MODE;
    commandLong.param1          = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    commandLong.param2          = Stabilize;
    commandLong.target_compid   = 0;    //default
    commandLong.target_sysid    = 0;    //default
    _mavlinkPassthrough->send_command_long(commandLong);
}

void ActionForArdupilot::setGuidedFlightModeToArdupilot(){
    mavsdk::MavlinkPassthrough::CommandLong commandLong;
    commandLong.command         = MAV_CMD_DO_SET_MODE;
    commandLong.param1          = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    commandLong.param2          = Guided;
    commandLong.target_compid   = 0;    //default
    commandLong.target_sysid    = 0;    //default
    _mavlinkPassthrough->send_command_long(commandLong);
}

ActionForPX4::ActionForPX4(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough, std::shared_ptr<mavsdk::Offboard> offboard)
    :CopilotAction(mavlinkPassthrough,offboard)
{

}

void ActionForPX4::takeOverControl(){
    _offboard->set_velocity_body({0,0,0,0});
    _offboard->start();
}

void ActionForPX4::giveAwayControl(){
    _offboard->set_velocity_body({0,0,0,0});
    _offboard->stop();
}
