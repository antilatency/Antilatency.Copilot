#include "TrackingForCopilot.h"

TrackingForCopilot::TrackingForCopilot(Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary, Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary, Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary)
{
    _deviceNetworkLibrary       = &deviceNetworkLibrary;
    _altTrackingLibrary         = &altTrackingLibrary;
    _environmentSelectorLibrary = &environmentSelectorLibrary;
}

bool TrackingForCopilot::createEnvironmentAndPlacement(std::string environmentData, std::string placementData)
{
    _environmentData = environmentData;
    _placementData = placementData;

    // Create environment object from the serialized data.
    _environment = _environmentSelectorLibrary->createEnvironment(_environmentData);
    if (_environment == nullptr) {
        return false;
    }

    // Create placement from the serialized data.
    _placement = _altTrackingLibrary->createPlacement(_placementData);

    return true;
}

bool TrackingForCopilot::createAltTrackingCotask()
{
    // Create a device network filter and then create a network using that filter.
    Antilatency::DeviceNetwork::IDeviceFilter filter = _deviceNetworkLibrary->createFilter();
    filter.addUsbDevice(Antilatency::DeviceNetwork::Constants::AllUsbDevices);
    _network = _deviceNetworkLibrary->createNetwork(filter);
    if (_network == nullptr) {
        std::cout << "Failed to create Antilatency Device Network" << std::endl;
        return false;
    }
    // Create alt tracking cotask constructor to find tracking-supported nodes and start tracking task on node.
    Antilatency::Alt::Tracking::ITrackingCotaskConstructor altTrackingCotaskConstructor = _altTrackingLibrary->createTrackingCotaskConstructor();
    if (altTrackingCotaskConstructor == nullptr) {
        std::cout << "Failed to create Antilatency Alt Tracking Cotask Constructor" << std::endl;
        return false;
    }

    // Each time the device network is changed due to connection or disconnection of a device that matches the device filter of the network,
    // or start or stop of a task on any network device, the network update id is incremented by 1.
    uint32_t prevUpdateId = 0;
    //std::cout << "Check network" <<
    while (_network != nullptr) {
        // Check if the network has been changed.
        const uint32_t currentUpdateId = _network.getUpdateId();

        if (prevUpdateId != currentUpdateId) {
            prevUpdateId = currentUpdateId;
            std::cout << "--- Device network changed, update id: " << currentUpdateId << " ---" << std::endl;

            // Get first idle node that supports tracking task.
            const Antilatency::DeviceNetwork::NodeHandle trackingNode = _getIdleTrackingNode(_network, altTrackingCotaskConstructor);

            if (trackingNode != Antilatency::DeviceNetwork::NodeHandle::Null) {
                // Start tracking task on node.
                _altTrackingCotask = altTrackingCotaskConstructor.startTask(_network, trackingNode, _environment);
                if (_altTrackingCotask != nullptr) {
                    return true;
                } else {
                    std::cout << "Failed to start tracking task on node" << std::endl;
                }
            }
        }else {
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }
    return false;
}

bool TrackingForCopilot::getState(Antilatency::Alt::Tracking::State &state){
    if(_network != nullptr){
        if (_altTrackingCotask != nullptr) {
            if (_altTrackingCotask.isTaskFinished()) {
                std::cout << "Tracking task finished" << std::endl;
                return false;
            } else {
                state = _altTrackingCotask.getExtrapolatedState(_placement,0.0f);
                return true;
            }
        } else {
            std::cout << "Failed tracking task on node" << std::endl;
            return false;
        }
    } else{
        std::cout << "Failed Antilatency Device Network for Tracking" << std::endl;
        return false;
    }
}

Antilatency::Alt::Tracking::State TrackingForCopilot::getState()
{
    Antilatency::Alt::Tracking::State state;
    getState(state);
    return state;
}

Antilatency::DeviceNetwork::NodeHandle TrackingForCopilot::_getIdleTrackingNode(Antilatency::DeviceNetwork::INetwork network, Antilatency::Alt::Tracking::ITrackingCotaskConstructor altTrackingCotaskConstructor) {
    // Get all currently connected nodes, that supports alt tracking task.
    std::vector<Antilatency::DeviceNetwork::NodeHandle> altNodes = altTrackingCotaskConstructor.findSupportedNodes(network);
    if (altNodes.size() == 0) {
        std::cout << "No nodes with Alt Tracking Task support found" << std::endl;
        return Antilatency::DeviceNetwork::NodeHandle::Null;
    }

    // Return first idle node.
    for (auto node : altNodes) {
        if (network.nodeGetStatus(node) == Antilatency::DeviceNetwork::NodeStatus::Idle) {
            return node;
        }
    }

    std::cout << "No idle nodes with Alt Tracking Task support found" << std::endl;
    return Antilatency::DeviceNetwork::NodeHandle::Null;
}
