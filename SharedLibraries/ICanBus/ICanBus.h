#ifndef ICANBUS_H
#define ICANBUS_H

#include "CANMessage.h"

class ICANBus {
public:
    virtual ~ICANBus() = default;

    virtual void SendMessage(CANMessage &message) = 0;

    virtual bool ReceiveAvailable() = 0;

    virtual void Receive() = 0;

    const uint8_t START_BYTE = 0xAA;
};

#endif // ICANBUS_H
