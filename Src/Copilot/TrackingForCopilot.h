#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>

#include <Antilatency.Api.h>
#include <Antilatency.InterfaceContract.LibraryLoader.h>

#include "CopilotMath.h"

class TrackingForCopilot
{
public:

    TrackingForCopilot(Antilatency::DeviceNetwork::ILibrary& deviceNetworkLibrary,
                       Antilatency::Alt::Tracking::ILibrary& altTrackingLibrary,
                       Antilatency::Alt::Environment::Selector::ILibrary& environmentSelectorLibrary);

    bool createEnvironmentAndPlacement(std::string environmentData, std::string placementData);
    bool createAltTrackingCotask();

    bool getState(Antilatency::Alt::Tracking::State &state);
    Antilatency::Alt::Tracking::State getState();

private:
    Antilatency::DeviceNetwork::NodeHandle _getIdleTrackingNode(Antilatency::DeviceNetwork::INetwork network, Antilatency::Alt::Tracking::ITrackingCotaskConstructor altTrackingCotaskConstructor);

    std::string _environmentData;
    std::string _placementData;
    Antilatency::Alt::Environment::IEnvironment _environment;
    Antilatency::Math::floatP3Q _placement;

    Antilatency::DeviceNetwork::INetwork _network;
    Antilatency::Alt::Tracking::ITrackingCotask _altTrackingCotask;

    Antilatency::DeviceNetwork::ILibrary* _deviceNetworkLibrary;
    Antilatency::Alt::Tracking::ILibrary* _altTrackingLibrary;
    Antilatency::Alt::Environment::Selector::ILibrary* _environmentSelectorLibrary;

    Antilatency::Alt::Tracking::State _state;
};
