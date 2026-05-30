#ifndef SELFOTA_H
#define SELFOTA_H

#include <functional>
#include "CanMessages.h"
#include "CanMessageIds.h"


class SelfOTA {
public:
    using StatusCallback = std::function<void(ECanNode, const CANAirRideOTAStatus &)>;

    void Init(StatusCallback cb);
    void Start();
    void Stop();
    void Handle();
    bool IsActive() const { return active; }

private:
    void Notify();

    static constexpr uint32_t AP_HEARTBEAT_MS = 5000;

    StatusCallback onStatus;
    bool active = false;
    CANAirRideOTAStatus currentStatus{};
    uint32_t lastHeartbeatMs = 0;
};

#endif // SELFOTA_H
