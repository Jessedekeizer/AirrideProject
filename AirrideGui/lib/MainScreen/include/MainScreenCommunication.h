#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "EMainScreenButtons.h"
#include "LogStorage.h"
#include "MainScreenData.h"

class MainScreenCommunication
{
public:
    MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData, LogStorage &logStorage);

    void Init();

    void Leave();

    void SendMessageButtonPress(EMainScreenButtons button, bool state);

private:
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    void HandlePressureMessage(const uint8_t *data, uint8_t length);
    void HandleLogMessage(const uint8_t *data, uint8_t length);

    String CreateLogMessage(bool front, float startPressure, float endPressure, float startTankPressure,
                            unsigned long time, bool direction, bool togetherMove);

    Communication &communication;
    MainScreenData &mainScreenData;
    LogStorage &logStorage;
    int communicationId;
};

#endif // MAINSCREENCOMMUNICATION_H
