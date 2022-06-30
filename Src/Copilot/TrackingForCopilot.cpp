#include "TrackingForCopilot.h"

TrackingForCopilot::TrackingForCopilot()
{

}

TrackingForCopilot::TrackingForCopilot(int updatePeriodDataInConsolms)
{
    _updatePeriodDataInConsolms = updatePeriodDataInConsolms;
    if(_updatePeriodDataInConsolms != 0){
        _timeBegin = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    }
}

Antilatency::Alt::Tracking::State TrackingForCopilot::getState(){
    return _state;
}

Antilatency::Math::float3 TrackingForCopilot::getPosition(){
    _mutex.lock();
    auto position = _position;
    _mutex.unlock();
    return position;
}

float TrackingForCopilot::getYaw(){
    return _yaw;
}

Antilatency::Alt::Tracking::Stability TrackingForCopilot::getStability(){
    return _stability;
}

bool TrackingForCopilot::isStateReady(){
    return _stateReady;
}

void TrackingForCopilot::setEnvironmentAndPlacementData(std::string environmentData, std::string placementData){
    _environmentData = environmentData;
    _placementData = placementData;
}

void TrackingForCopilot::setLibName(std::string libNameADN, std::string libNameTracking, std::string libNameEnvironmentSelector){
    _libNameADN = libNameADN;
    _libNameTracking = libNameTracking;
    _libNameEnvironmentSelector = libNameEnvironmentSelector;
}

bool TrackingForCopilot::beginTracking(){
    // Load the Antilatency Device Network library
    Antilatency::DeviceNetwork::ILibrary deviceNetworkLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::DeviceNetwork::ILibrary>(_libNameADN.c_str());
    if (deviceNetworkLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Device Network Library" << std::endl;
        return 1;
    }

    // Load the Antilatency Alt Tracking library
    Antilatency::Alt::Tracking::ILibrary altTrackingLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Tracking::ILibrary>(_libNameTracking.c_str());
    if (altTrackingLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Tracking Library" << std::endl;
        return 1;
    }

    // Load the Antilatency Alt Environment Selector library
    Antilatency::Alt::Environment::Selector::ILibrary environmentSelectorLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Environment::Selector::ILibrary>(_libNameEnvironmentSelector.c_str());
    if (environmentSelectorLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Environment Selector Library" << std::endl;
        return 1;
    }

    // Create a device network filter and then create a network using that filter.
    Antilatency::DeviceNetwork::IDeviceFilter filter = deviceNetworkLibrary.createFilter();
    filter.addUsbDevice(Antilatency::DeviceNetwork::Constants::AllUsbDevices);
    Antilatency::DeviceNetwork::INetwork network = deviceNetworkLibrary.createNetwork(filter);
    if (network == nullptr) {
        std::cout << "Failed to create Antilatency Device Network" << std::endl;
        return 1;
    }

    // Create environment object from the serialized data.
    const Antilatency::Alt::Environment::IEnvironment environment = environmentSelectorLibrary.createEnvironment(_environmentData);
    if (environment == nullptr) {
        std::cout << "Failed to create environment" << std::endl;
        return 1;
    }

    // Create placement from the serialized data.
    const Antilatency::Math::floatP3Q placement = altTrackingLibrary.createPlacement(_placementData);

    // Create alt tracking cotask constructor to find tracking-supported nodes and start tracking task on node.
    Antilatency::Alt::Tracking::ITrackingCotaskConstructor altTrackingCotaskConstructor = altTrackingLibrary.createTrackingCotaskConstructor();
    if (altTrackingCotaskConstructor == nullptr) {
        std::cout << "Failed to create Antilatency Alt Tracking Cotask Constructor" << std::endl;
        return 1;
    }

    // Each time the device network is changed due to connection or disconnection of a device that matches the device filter of the network,
    // or start or stop of a task on any network device, the network update id is incremented by 1.
    uint32_t prevUpdateId = 0;
    //std::cout << "Check network" <<
    while (network != nullptr) {
        // Check if the network has been changed.
        const uint32_t currentUpdateId = network.getUpdateId();

        if (prevUpdateId != currentUpdateId) {
            prevUpdateId = currentUpdateId;
            std::cout << "--- Device network changed, update id: " << currentUpdateId << " ---" << std::endl;

            // Get first idle node that supports tracking task.
            const Antilatency::DeviceNetwork::NodeHandle trackingNode = _getIdleTrackingNode(network, altTrackingCotaskConstructor);

            if (trackingNode != Antilatency::DeviceNetwork::NodeHandle::Null) {
                // Start tracking task on node.
                Antilatency::Alt::Tracking::ITrackingCotask altTrackingCotask = altTrackingCotaskConstructor.startTask(network, trackingNode, environment);
                if (altTrackingCotask != nullptr) {

                    while (altTrackingCotask != nullptr) {

                        if (altTrackingCotask.isTaskFinished()) {
                            std::cout << "Tracking task finished" << std::endl;
                            break;
                        }

                        _state = altTrackingCotask.getExtrapolatedState(placement, 0.0f);

                        _mutex.lock();
                        _position = _state.pose.position;
                        _mutex.unlock();

                        _angleDegreesFromAlt = quaternionAltToAngleDronePX4(_state.pose.rotation,0);

                        _yaw = _angleDegreesFromAlt.yawDeg;

                        _stability = _state.stability;

                        if(_stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
                            _stateReady = true;
                        }

                        if(_updatePeriodDataInConsolms!=0){
                            _timeNow = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
                            if((int)(_timeNow -_timeBegin)>=_updatePeriodDataInConsolms){
                                _timeBegin = _timeNow;
                                if(_stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
                                    std::cout << "\nTracking Data" << std::endl;
                                    std::cout << "Position\tx: " << _state.pose.position.x << ", z: " << _state.pose.position.z << ", y: " << _state.pose.position.y << std::endl;
                                    std::cout << "Yaw:\t"      << _yaw << std::endl;
                                } else {
                                    std::cout << "\nAlt:  Don't see environment" << std::endl;
                                }
                                std::cout << "Stability stage:\t" << Antilatency::enumToString(_state.stability.stage) << std::endl;
                            }
                        }

                        std::this_thread::sleep_for(std::chrono::microseconds(1002));
                    }
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
