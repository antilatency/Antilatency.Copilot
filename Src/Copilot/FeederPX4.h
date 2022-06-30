#pragma once
#include "PX4.h"
#include "TrackingForCopilot.h"
#include "CopilotAngles.h"

class FeederPX4
{
public:
    FeederPX4(PX4& px4, TrackingForCopilot& tracking);

    bool feedPX4DataFromTracking();

private:
    PX4* _px4;
    TrackingForCopilot* _tracking;
};
