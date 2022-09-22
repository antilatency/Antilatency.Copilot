#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <optional>

#include <Antilatency.Api.h>
#include <Antilatency.InterfaceContract.LibraryLoader.h>

class Application
{
public:
    Application(Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary,
                Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary,
                Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary,
                std::string environmentData, std::string placementData);

    virtual void run() = 0;

protected:
    std::optional<Antilatency::Alt::Tracking::State> getTrackingState();

private:
    bool startTracking();

    Antilatency::Math::floatP3Q _placement;
    Antilatency::Alt::Environment::IEnvironment _environment;
    Antilatency::DeviceNetwork::INetwork _network = nullptr;
    Antilatency::Alt::Tracking::ITrackingCotaskConstructor _altTrackingCotaskConstructor= nullptr;
    Antilatency::Alt::Tracking::ITrackingCotask _altTrackingCotask = nullptr;
    uint32_t _lastUpdateId=0;
};
