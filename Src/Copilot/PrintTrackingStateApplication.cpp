#include "PrintTrackingStateApplication.h"

PrintTrackingStateApplication::PrintTrackingStateApplication(int periodOfPrintingTrackingStateSec, Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary, Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary, Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary, std::string environmentData, std::string placementData):
    Application(deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary,environmentData,placementData),
    _periodOfPrintingTrackingStateSec{periodOfPrintingTrackingStateSec}
{

}

void PrintTrackingStateApplication::run() {
    auto timeBegin = std::chrono::system_clock::now();
    while(true){
        if(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now()-timeBegin).count() >= _periodOfPrintingTrackingStateSec){
            timeBegin = std::chrono::system_clock::now();
            printTrackingState();
        }
        std::this_thread::yield();
    }
}

void PrintTrackingStateApplication::printTrackingState(){
    auto trackingState = getTrackingState();
    if(trackingState.has_value()){
        if(trackingState->stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
            std::cout << "\nTracking Data" << std::endl;
            std::cout << "Position\tx: " << trackingState->pose.position.x << ", z: " << trackingState->pose.position.z << ", y: " << trackingState->pose.position.y << std::endl;
            auto angleDegreesFromAlt = quaternionAltToAngleDronePX4(trackingState->pose.rotation);
            std::cout << "Yaw:\t"      << angleDegreesFromAlt.yawDeg << std::endl;
        } else {
            std::cout << "\nDon't see environment" << std::endl;
        }
        std::cout << "Stability stage:\t" << Antilatency::enumToString(trackingState->stability.stage) << std::endl;
    } else {
        std::cout << "\nTracking state not available now!\n" <<std::endl;
    }
}
