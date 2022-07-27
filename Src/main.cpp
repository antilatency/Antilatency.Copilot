#include <iostream>

#include <dlfcn.h>

#include "Copilot/PX4.h"
#include "Copilot/TrackingForCopilot.h"
#include "Copilot/TargetLanding.h"
#include <thread>
#include <chrono>

std::string getParentPath(const char *inp){
    auto len = strlen(inp);
    if(len == 0) throw std::runtime_error("no parent path: " + std::string(inp));
    int i = len-1;
    while(i > 0){
        if(inp[i] == '/'){
            return std::string(inp, inp + i + 1);
        }
        --i;
    }
    throw std::runtime_error("no parent path: " + std::string(inp));
}

int main(int argc, char* argv[]) {

    Dl_info dlinfo;
    dladdr(reinterpret_cast<void*>(&main), &dlinfo);
    std::string path = getParentPath(dlinfo.dli_fname);
    std::string libNameADN =path + "libAntilatencyDeviceNetwork.so";
    std::string libNameTracking =path + "libAntilatencyAltTracking.so";
    std::string libNameEnvironmentSelector =path + "libAntilatencyAltEnvironmentSelector.so";

    // Load the Antilatency Device Network library
    Antilatency::DeviceNetwork::ILibrary deviceNetworkLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::DeviceNetwork::ILibrary>(libNameADN.c_str());
    if (deviceNetworkLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Device Network Library" << std::endl;
        return 1;
    }
    // Load the Antilatency Alt Tracking library
    Antilatency::Alt::Tracking::ILibrary altTrackingLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Tracking::ILibrary>(libNameTracking.c_str());
    if (altTrackingLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Tracking Library" << std::endl;
        return 1;
    }
    // Load the Antilatency Alt Environment Selector library
    Antilatency::Alt::Environment::Selector::ILibrary environmentSelectorLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Environment::Selector::ILibrary>(libNameEnvironmentSelector.c_str());
    if (environmentSelectorLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Environment Selector Library" << std::endl;
        return 1;
    }

    /*First Mode*/
    if(argc == 3)
    {
       std::cout << "\nYou run only Tracking to see coordinat of landing target\n" << std::endl;
       const std::string environmentData = argv[1];
       const std::string placementData = argv[2];
       std::cout << "You set:"
                 << "\nas environmentData:\t"      << environmentData
                 << "\nas placementData:\t"        << placementData << std::endl;

       TrackingForCopilot tracking(deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary);

       if(!tracking.createEnvironmentAndPlacement(environmentData,placementData)){
           std::cout << "Failed to create environment" << std::endl;
           return 1;
       }

       if(!tracking.createAltTrackingCotask()){
           std::cout << "Failed to create Alt Tracking Cotask" << std::endl;
           return 1;
       }

       Antilatency::Alt::Tracking::State state;

       while(true){
           if(tracking.getState(state)){
               if(state.stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
                   std::cout << "\nTracking Data" << std::endl;
                   std::cout << "Position\tx: " << state.pose.position.x << ", z: " << state.pose.position.z << ", y: " << state.pose.position.y << std::endl;
                   auto angleDegreesFromAlt = quaternionAltToAngleDronePX4(state.pose.rotation);
                   std::cout << "Yaw:\t"      << angleDegreesFromAlt.yawDeg << std::endl;
               } else {
                   std::cout << "\nAlt:  Don't see environment" << std::endl;
               }
               std::cout << "Stability stage:\t" << Antilatency::enumToString(state.stability.stage) << std::endl;
           } else {
               return 1;
           }
           std::this_thread::sleep_for(std::chrono::seconds(1));
       }
       return 0;
    } else if(argc != 10){
        std::cout << "Wrong arguments. Pass environment data string as first argument and placement data as second.\n"
                  << "If you just whant to see the tracking data.\n\n"
                  << "If you whant run AntilatencyCopilotDemo whith target landing you must pass as next argument X(3),Z(4),Y(5),YAW(6) landing point coordinates,\n"
                  << "safe altitude for movement to the landing poitn (7),\n"
                  << "coordinate control precision during landing for XZ(8) and YAW(9) in degrees\n" << std::endl;
        return 1;
    }


    /*Second Mode*/
    const std::string environmentData = argv[1];
    const std::string placementData   = argv[2];

    float landTargetX   = std::stof(argv[3]);
    float landTargetZ   = std::stof(argv[4]);
    float landTargetY   = std::stof(argv[5]);
    float landTargetYAW = std::stof(argv[6]);
    float safeAltitude  = std::stof(argv[7]);
    float precisionPose = std::stof(argv[8]);
    float precisionYaw  = std::stof(argv[9]);

    std::cout << "You set:"
              << "\nas environmentData:\t"      << environmentData
              << "\nas placementData:\t"        << placementData
              << "\nas X of land Target:\t"     << landTargetX
              << "\nas Z of land Target:\t"     << landTargetZ
              << "\nas Y of land Target:\t"     << landTargetY
              << "\nas YAW of land Target:\t"   << landTargetYAW
              << "\nas safe Altitude:\t"        << safeAltitude
              << "\nas precision for pose:\t"   << precisionPose
              << "\nas precision for yaw:\t"    << precisionYaw << "\n" << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    TrackingForCopilot tracking(deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary);

    if(!tracking.createEnvironmentAndPlacement(environmentData,placementData)){
        std::cout << "Failed to create environment" << std::endl;
        return 1;
    }

    if(!tracking.createAltTrackingCotask()){
        std::cout << "Failed to create Alt Tracking Cotask" << std::endl;
        return 1;
    }

    PX4 autoPilot;
    std::thread linkToAutoPilot([&](){
         autoPilot.connectToPX4();
         autoPilot.createPluginsPX4();
     });

    while((tracking.getState().stability.stage !=  Antilatency::Alt::Tracking::Stage::Tracking6Dof) || (!autoPilot.isConnected())){
        std::cout << std::endl;
        if(tracking.getState().stability.stage !=  Antilatency::Alt::Tracking::Stage::Tracking6Dof){
            std::cout << "Tracking is not ready yet" << std::endl;
        } else {
            std::cout << "Tracking is ready!" << std::endl;
        }

        if((!autoPilot.isConnected())){
            std::cout << "Autopilot is not ready yet" << std::endl;
        } else {
            std::cout << "Autopilot is ready!" << std::endl;
        }
        std::cout << std::endl;
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }

    std::cout << "\nTracking is ready!" << std::endl;
    std::cout << "Autopilot is ready!\n" << std::endl;

    TargetLanding targetLanding(autoPilot,tracking);
    bool waitForLandingCommand = true;
    bool takeOverAndLand  = false;

    std::cout << "Wait for landing command" << std::endl;

    while(true){
        if((tracking.getState().stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof) && autoPilot.isConnected()){
            autoPilot.setTrackerData(convertAltPoseToVisionPositionEstimate(tracking.getState().pose));
        }

        if(autoPilot.isLanding() && waitForLandingCommand && autoPilot.isArming()){
            std::cout << "Landin command recive" << std::endl;
            if(tracking.getState().stability.stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
                if(autoPilot.offboardStart()){
                    std::cout << "Start Offboard mode" << std::endl;
                    takeOverAndLand = true;
                    waitForLandingCommand = false;
                } else {
                    std::cout << "Can't take over and land - Offboard mode faild" << std::endl;
                }
            } else{
                std::cout << "Can't take over and land - don't see environment" << std::endl;
            }
        }
        if(takeOverAndLand){
            targetLanding.executingTargetLanding(landTargetX,landTargetZ,landTargetY,landTargetYAW,safeAltitude,precisionYaw,precisionPose,waitForLandingCommand,takeOverAndLand);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return 0;
}
