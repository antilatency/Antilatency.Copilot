#include <iostream>
#include <chrono>
#include <thread>
#include <future>

#include <dlfcn.h>

#include <Antilatency.Api.h>
#include <Antilatency.InterfaceContract.LibraryLoader.h>

#include "Copilot/PrintTrackingStateApplication.h"
#include "Copilot/TargetLandingApplication.h"

std::string getApplicationDirectory() {
    Dl_info dlinfo;
    dladdr(reinterpret_cast<void*>(&getApplicationDirectory), &dlinfo);
    auto inp = dlinfo.dli_fname;
    auto len = strlen(inp);
    int i = len-1;
    while(i > 0){
        if(inp[i] == '/'){
            return std::string(dlinfo.dli_fname, dlinfo.dli_fname + i + 1);
        }
        --i;
    }
}

bool loadAntilatencyLibraries(Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary,
                           Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary,
                           Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary){
    auto applicationDirectory = getApplicationDirectory();
    std::string libNameADN = applicationDirectory + "libAntilatencyDeviceNetwork.so";
    std::string libNameTracking = applicationDirectory + "libAntilatencyAltTracking.so";
    std::string libNameEnvironmentSelector = applicationDirectory + "libAntilatencyAltEnvironmentSelector.so";

    deviceNetworkLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::DeviceNetwork::ILibrary>(libNameADN.c_str());
    if (deviceNetworkLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Device Network Library" << std::endl;
        return false;
    }

    altTrackingLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Tracking::ILibrary>(libNameTracking.c_str());
    if (altTrackingLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Tracking Library" << std::endl;
        return false;
    }

    environmentSelectorLibrary = Antilatency::InterfaceContract::getLibraryInterface<Antilatency::Alt::Environment::Selector::ILibrary>(libNameEnvironmentSelector.c_str());
    if (environmentSelectorLibrary == nullptr) {
        std::cout << "Failed to get Antilatency Alt Environment Selector Library" << std::endl;
        return false;
    }
    return true;
}

void printInfo(){
    std::cout << "\nWrong arguments. \n\nPass environment data string as first argument and placement data as second.\n"
                  << "If you just whant to see the tracking data.\n\n"
                  << "If you whant run AntilatencyCopilotDemo whith target landing\n"
                  << "you must pass as next argument X(3),Z(4),Y(5),YAW(6) landing point coordinates,"
                  << "safe altitude for movement to the landing poitn (7),\n"
                  << "coordinate control precision during landing for XZ(8) and YAW(9) in degrees\n" << std::endl;
}

void printInfo(std::string environmentData, std::string placementData){
    std::cout << "\nYou run only tracking to see coordinates of landing target\n" << std::endl;
    std::cout << "You set:"
              << "\nas environmentData:\t"      << environmentData
              << "\nas placementData:\t"        << placementData << "\n" <<std::endl;
}

void printInfo(std::string environmentData, std::string placementData, TargetLandingApplication::LandingTargetProperties landingTargetProperties){
    std::cout << "\nYou run feed autopilot tracking data and take over control for landing to the target when \"land\" command will coming)\n" << std::endl;
    std::cout << "You set:"
                  << "\nas environmentData:\t"      << environmentData
                  << "\nas placementData:\t"        << placementData
                  << "\nas X of land Target:\t"     << landingTargetProperties.position.x
                  << "\nas Z of land Target:\t"     << landingTargetProperties.position.z
                  << "\nas Y of land Target:\t"     << landingTargetProperties.position.y
                  << "\nas YAW of land Target:\t"   << landingTargetProperties.yaw
                  << "\nas safe Altitude:\t"        << landingTargetProperties.safeAltitude
                  << "\nas precision for pose:\t"   << landingTargetProperties.precisionForPose
                  << "\nas precision for yaw:\t"    << landingTargetProperties.precisionForYaw << "\n" << std::endl;
}

int main(int argc, char* argv[]) {

    if(!((argc==3) || (argc ==10))){
        printInfo();
        return 0;
    }

    Antilatency::DeviceNetwork::ILibrary deviceNetworkLibrary;
    Antilatency::Alt::Tracking::ILibrary altTrackingLibrary;
    Antilatency::Alt::Environment::Selector::ILibrary environmentSelectorLibrary;

    bool result = loadAntilatencyLibraries(deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary);
    if(result == false){
        return 1;
    }

    std::string environmentData = argv[1];
    std::string placementData   = argv[2];

    int periodOfPrintingTrackingStateSec = 1;
    TargetLandingApplication::LandingTargetProperties landingTargetProperties;
    try{
        switch (argc) {
        case 3:{
            printInfo(environmentData, placementData);
            PrintTrackingStateApplication printTrackerState(periodOfPrintingTrackingStateSec,deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary,environmentData,placementData);
            return printTrackerState.run();
        } break;
        case 10:{
            landingTargetProperties.position.x      = std::stof(argv[3]);
            landingTargetProperties.position.z      = std::stof(argv[4]);
            landingTargetProperties.position.y      = std::stof(argv[5]);
            landingTargetProperties.yaw             = std::stof(argv[6]);
            landingTargetProperties.safeAltitude    = std::stof(argv[7]);
            landingTargetProperties.precisionForPose= std::stof(argv[8]);
            landingTargetProperties.precisionForYaw = std::stof(argv[9]);

            printInfo(environmentData,placementData,landingTargetProperties);
            TargetLandingApplication targetLanding(landingTargetProperties,deviceNetworkLibrary,altTrackingLibrary,environmentSelectorLibrary,environmentData,placementData);
            return targetLanding.run();
        } break;
        default:{
            printInfo();
        } break;
        }
    } catch (std::exception& error){
        std::cerr << error.what() << std::endl;
    }
    return 0;
}
