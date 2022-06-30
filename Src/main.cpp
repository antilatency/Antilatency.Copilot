#include <iostream>

#include <dlfcn.h>

#include "Copilot/PX4.h"
#include "Copilot/TrackingForCopilot.h"
#include "Copilot/FeederPX4.h"
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

    if(argc == 3)
    {
       std::cout << "\nYou run only Tracking to see coordinat of landing target\n" << std::endl;
       const std::string environmentData = argv[1];
       const std::string placementData = argv[2];
       std::cout << "You set:"
                 << "\nas environmentData:\t"      << environmentData
                 << "\nas placementData:\t"        << placementData << std::endl;
       int updatePeriodDataInConsolms = 1000;
       TrackingForCopilot tracking(updatePeriodDataInConsolms);
       Dl_info dlinfo;
       dladdr(reinterpret_cast<void*>(&main), &dlinfo);
       std::string path = getParentPath(dlinfo.dli_fname);
       std::string libNameADN =path + "libAntilatencyDeviceNetwork.so";
       std::string libNameTracking =path + "libAntilatencyAltTracking.so";
       std::string libNameEnvironmentSelector =path + "libAntilatencyAltEnvironmentSelector.so";

       tracking.setEnvironmentAndPlacementData(environmentData,placementData);
       tracking.setLibName(libNameADN,libNameTracking,libNameEnvironmentSelector);
       tracking.beginTracking();
       return 0;
    }

    if(argc != 10){
        std::cout << "Wrong arguments. Pass environment data string as first argument and placement data as second.\n"
                  << "If you just whant to see the tracking data.\n\n"
                  << "If you whant run AntilatencyCopilotDemo whith target landing you must pass as next argument X(3),Z(4),Y(5),YAW(6) landing point coordinates,\n"
                  << "safe altitude for movement to the landing poitn (7),\n"
                  << "coordinate control precision during landing for XZ(8) and YAW(9) in degrees\n" << std::endl;
        return 1;
    }

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

    PX4 autoPilot;
    std::thread linkToAutoPilot([&](){
        autoPilot.connectToPX4();
        autoPilot.createPluginsPX4();
    });

    int updatePeriodDataInConsolms = 5000;
    TrackingForCopilot tracking(updatePeriodDataInConsolms);
    Dl_info dlinfo;
    dladdr(reinterpret_cast<void*>(&main), &dlinfo);
    std::string path = getParentPath(dlinfo.dli_fname);
    std::string libNameADN =path + "libAntilatencyDeviceNetwork.so";
    std::string libNameTracking =path + "libAntilatencyAltTracking.so";
    std::string libNameEnvironmentSelector =path + "libAntilatencyAltEnvironmentSelector.so";

    std::thread linkToAlt([&](){
        tracking.setEnvironmentAndPlacementData(environmentData,placementData);
        tracking.setLibName(libNameADN,libNameTracking,libNameEnvironmentSelector);
        tracking.beginTracking();
    });

    while((!tracking.isStateReady()) || (!autoPilot.isConnected())){
        std::cout << std::endl;
        if((!tracking.isStateReady())){
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

    FeederPX4 feederPX4(autoPilot,tracking);
    std::thread feedPX4TrackingData([&](){
        feederPX4.feedPX4DataFromTracking();
    });

    bool waitForLandingCommand = true;
    bool takeOverAndLand  = false;

    TargetLanding targetLanding(autoPilot,tracking);

    std::cout << "Wait for landing command" << std::endl;
    while(true){
        if(autoPilot.isLanding() && waitForLandingCommand && autoPilot.isArming()){
            std::cout << "Landin command recive" << std::endl;
            if(tracking.getStability().stage == Antilatency::Alt::Tracking::Stage::Tracking6Dof){
                if(autoPilot.offboardStart_()){
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
    }
    return 0;
}
