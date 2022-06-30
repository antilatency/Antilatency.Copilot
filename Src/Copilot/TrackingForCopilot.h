#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include <Antilatency.Api.h>
#include <Antilatency.InterfaceContract.LibraryLoader.h>

#include "CopilotAngles.h"

class TrackingForCopilot
{
public:

    TrackingForCopilot();
    TrackingForCopilot(int updatePeriodDataInConsolms);

    void setEnvironmentAndPlacementData(std::string environmentData,std::string placementData);
    void setLibName(std::string libNameADN, std::string libNameTracking, std::string libNameEnvironmentSelector);
    bool beginTracking();

    Antilatency::Alt::Tracking::State getState();
    Antilatency::Math::float3 getPosition();
    Antilatency::Alt::Tracking::Stability getStability();
    float getYaw();

    bool isStateReady();

private:
    Antilatency::DeviceNetwork::NodeHandle _getIdleTrackingNode(Antilatency::DeviceNetwork::INetwork network, Antilatency::Alt::Tracking::ITrackingCotaskConstructor altTrackingCotaskConstructor);

    std::string _environmentData;
    std::string _placementData;
    std::string _libNameADN;
    std::string _libNameTracking;
    std::string _libNameEnvironmentSelector;

    std::mutex _mutex;

    Antilatency::Alt::Tracking::State _state;
    Antilatency::Alt::Tracking::Stability _stability;
    Antilatency::Math::float3 _position;
    AnglesInDegrees _angleDegreesFromAlt;
    float _yaw   = NAN;
    bool  _stateReady =  false;

    long long _timeBegin;
    long long _timeNow;
    int _updatePeriodDataInConsolms = 0;
};
