#include "Application.h"

Application::Application(Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary, Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary, Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary, std::string environmentData, std::string placementData)
{
    _environment = environmentSelectorLibrary.createEnvironment(environmentData);
    if (_environment == nullptr) {
        throw std::runtime_error("Failed to create environment");
    }
    _placement = altTrackingLibrary.createPlacement(placementData);

    auto filter = deviceNetworkLibrary.createFilter();
    filter.addUsbDevice(Antilatency::DeviceNetwork::Constants::AllUsbDevices);
    _network = deviceNetworkLibrary.createNetwork(filter);
    if (_network == nullptr) {
        throw std::runtime_error("Failed to create Antilatency Device Network");
    }
    _altTrackingCotaskConstructor = altTrackingLibrary.createTrackingCotaskConstructor();
    if (_altTrackingCotaskConstructor == nullptr) {
        throw std::runtime_error("Failed to create Antilatency Alt Tracking Cotask Constructor");
    }
}

bool Application::startTracking(){
    if(_lastUpdateId != _network.getUpdateId()){
        _lastUpdateId = _network.getUpdateId();
        auto altNodes = _altTrackingCotaskConstructor.findSupportedNodes(_network);
        for (auto node : altNodes) {
            if (_network.nodeGetStatus(node) == Antilatency::DeviceNetwork::NodeStatus::Idle) {
                _altTrackingCotask = _altTrackingCotaskConstructor.startTask(_network, node, _environment);
                if (_altTrackingCotask != nullptr) {
                    return true;
                }
            }
        }
    }
    return false;
}

std::optional<Antilatency::Alt::Tracking::State> Application::getTrackingState(){
    if(_altTrackingCotask != nullptr){
        if(_altTrackingCotask.isTaskFinished()){
            _altTrackingCotask = nullptr;
            return std::nullopt;
        }
    }else {
        if(!startTracking()){
            return std::nullopt;
        }
    }

    return _altTrackingCotask.getExtrapolatedState(_placement,0.0f);
}
