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

    StatusCallback onStatus;
    bool active = false;
    CANAirRideOTAStatus currentStatus{};
};

#endif // SELFOTA_H
