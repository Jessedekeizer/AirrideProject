#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "EMainScreenButtons.h"
#include "LogStorage.h"
#include "MainScreenData.h"

class MainScreenCommunication {
public:
    MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData, LogStorage &logStorage);

    void Init();

    void Leave();

    void SendMessageButtonPress(EMainScreenButtons button, bool state);

private:
    void ReceiveCallback(CANMessage &message);

    void HandlePressureMessage(CANMessage &message);

    Communication &communication;
    MainScreenData &mainScreenData;
    LogStorage &logStorage;
    int communicationId;
};

#endif //MAINSCREENCOMMUNICATION_H
