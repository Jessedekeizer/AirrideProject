#ifndef OTACOMMUNICATION_H
#define OTACOMMUNICATION_H
#include "Communication.h"
#include "CanMessages.h"

class OTACommunication {
public:
    OTACommunication(Communication &communication);
    void Init();
    void Leave();
    void Loop();

private:
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);
    void SendStatus(EOTAStatusType type, EOTAUpdatePhase phase, uint8_t progress);

    Communication &communication;
    int subscriptionId = -1;
    bool updating = false;
    bool startSent = false;
    uint8_t lastSentProgress = 255;
    unsigned long updateStartMs = 0;
    uint8_t failAtProgress = 255;

    static constexpr unsigned long FAKE_DURATION_MS = 5000;
};

#endif // OTACOMMUNICATION_H
