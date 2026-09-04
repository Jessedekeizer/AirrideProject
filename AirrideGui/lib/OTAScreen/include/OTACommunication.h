#ifndef OTACOMMUNICATION_H
#define OTACOMMUNICATION_H

#include <functional>
#include "Communication.h"
#include "CanMessageIds.h"
#include "CanMessages.h"
#include "SelfOTA.h"

class OTACommunication {
public:
    using StatusCallback = std::function<void(ECanNode node, const CANAirRideOTAStatus &status)>;

    explicit OTACommunication(Communication &communication);

    void Init(StatusCallback onStatus);
    void Leave();
    void Handle();

    void SendDiscover();
    void SendStart(ECanNode target);
    void SendStop(ECanNode target);

    bool IsLocalActive() const { return selfOta.IsActive(); }

private:
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);
    void HandleAck(const uint8_t *data, uint8_t length);

    Communication &communication;
    StatusCallback onStatus;
    SelfOTA selfOta;
    int subscriptionId = -1;
};

#endif // OTACOMMUNICATION_H
