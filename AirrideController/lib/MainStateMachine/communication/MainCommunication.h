#ifndef MAINCOMMUNICATION_H
#define MAINCOMMUNICATION_H
#include "Communication.h"
#include "Settings.h"
#include "MainStateMachineData.h"
#include "OTACommunication.h"

class MainCommunication {
public:
    MainCommunication(Communication &communication, Settings &settings,
                      MainStateMachineData &controllerData, OTACommunication &otaCommunication);

    void Init();
    void Leave();

private:
    void SaveSettings(const uint8_t *data, uint8_t length);
    void HandleOTACommand(const uint8_t *data, uint8_t length);
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    Communication &communication;
    Settings &settings;
    MainStateMachineData &controllerData;
    OTACommunication &otaCommunication;
    int communicationId = -1;
};

#endif //MAINCOMMUNICATION_H
