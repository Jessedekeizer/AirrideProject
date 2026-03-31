#ifndef MAINCOMMUNICATION_H
#define MAINCOMMUNICATION_H
#include "Communication.h"
#include "Settings.h"
#include "WString.h"

class MainCommunication {
public:
    MainCommunication(Communication &communication, Settings &settings);

    void SendPressure(float front, float back);

    void Init();

    void Leave();

private:
    void SaveSettings(const uint8_t *data, uint8_t length);

    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    Communication &communication;
    Settings &settings;
    int communicationId;
};

#endif //MAINCOMMUNICATION_H
