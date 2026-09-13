#ifndef OTACOMMUNICATION_H
#define OTACOMMUNICATION_H

#include <functional>

#include "CanMessageIds.h"
#include "CanMessages.h"
#include "Communication.h"
#include "SelfOTA.h"

class OTACommunication {
public:
    using StatusCallback = std::function<void(ECanNode node, const CANAirRideOTAStatus &status)>;

    explicit OTACommunication(Communication &communication);

    void Init(StatusCallback onStatus);

    void Subscribe();

    void Unsubscribe();

    void SetPrepareLocalCallback(std::function<void()> callback) { prepareLocal = callback; }

    void Handle();

    void SendDiscover();

    void SendStart(ECanNode target);

    void SendStop(ECanNode target);

    bool IsLocalActive() const { return selfOta.IsActive(); }

    bool IsPumping() const { return pumping; }

private:
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    void HandleAck(const uint8_t *data, uint8_t length);

    Communication &communication;
    StatusCallback onStatus;
    SelfOTA selfOta;
    int subscriptionId = -1;
    bool pumping = false;
    std::function<void()> prepareLocal;
};

#endif
