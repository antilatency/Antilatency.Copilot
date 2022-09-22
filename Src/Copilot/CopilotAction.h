#pragma once
#include <mavsdk/plugins/offboard/offboard.h>
#include <mavsdk/plugins/mavlink_passthrough/mavlink_passthrough.h>

class CopilotAction
{
public:
    CopilotAction(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough,std::shared_ptr<mavsdk::Offboard> offboard);

    virtual void takeOverControl()=0;
    virtual void giveAwayControl()=0;

protected:
    std::shared_ptr<mavsdk::MavlinkPassthrough> _mavlinkPassthrough;
    std::shared_ptr<mavsdk::Offboard> _offboard;
};

class ActionForArdupilot: public CopilotAction{
public:
    ActionForArdupilot(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough,std::shared_ptr<mavsdk::Offboard> offboard);

    void takeOverControl() override;;
    void giveAwayControl() override;

    enum FlightModeOfArdupilot {
        Stabilize = 0,
        Acro = 1,
        AltHold = 2,
        Auto = 3,
        Guided = 4,
        Loiter = 5,
        RTL = 6,
        Circle = 7,
        Land = 9,
        Drift = 11,
        Sport = 13,
        Flip = 14,
        AutoTune = 15,
        PosHold = 16,
        Break = 17,
        Throw = 18,
        Avoid_ADBS = 19,
        Guided_NoGPS = 20,
        Smart_RTL = 21,
        FlowHold = 22,
        Follow = 23,
        Zigzag = 24,
        SystemID = 25,
        Heli_Autorotate = 26,
        AutoRTL = 27,
    };

private:
    void setStabilizeFlightModeToArdupilot();
    void setGuidedFlightModeToArdupilot();
};

class ActionForPX4 :public CopilotAction{
public:
    ActionForPX4(std::shared_ptr<mavsdk::MavlinkPassthrough> mavlinkPassthrough,std::shared_ptr<mavsdk::Offboard> offboard);

    void takeOverControl() override;;
    void giveAwayControl() override;;
};
