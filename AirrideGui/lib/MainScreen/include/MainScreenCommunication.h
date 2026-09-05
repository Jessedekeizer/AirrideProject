#ifndef MAINSCREENCOMMUNICATION_H
#define MAINSCREENCOMMUNICATION_H
#include "Communication.h"
#include "EMainScreenButtons.h"
#include "LogStorage.h"
#include "MainScreenData.h"
#include "SettingsDevice.h"

class MainScreenCommunication
{
public:
    MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData, LogStorage &logStorage,
                            SettingsDevice &settings);

    void Init();

    void Leave();

    void SendMessageButtonPress(EMainScreenButtons button, bool state);

    void SendSettings();

private:
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    void HandlePressureMessage(const uint8_t *data, uint8_t length);
    void HandleLogMessage(const uint8_t *data, uint8_t length);
    void HandleAck(const uint8_t *data, uint8_t length);

    String CreateLogMessage(bool front, float startPressure, float endPressure, float startTankPressure,
                            unsigned long time, bool direction, bool togetherMove);

    Communication &communication;
    MainScreenData &mainScreenData;
    LogStorage &logStorage;
    SettingsDevice &settings;
    int communicationId;
};

#endif // MAINSCREENCOMMUNICATION_H
