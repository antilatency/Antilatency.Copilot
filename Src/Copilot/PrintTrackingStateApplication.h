#pragma once
#include "Application.h"
#include "CopilotMath.h"

class PrintTrackingStateApplication : public Application
{
public:
    PrintTrackingStateApplication(int periodOfPrintingTrackingStateSec,Antilatency::DeviceNetwork::ILibrary &deviceNetworkLibrary,
                        Antilatency::Alt::Tracking::ILibrary &altTrackingLibrary,
                        Antilatency::Alt::Environment::Selector::ILibrary &environmentSelectorLibrary,
                        std::string environmentData, std::string placementData);
   void run() override;

protected:
    void printTrackingState();

private:
    int _periodOfPrintingTrackingStateSec;

};

