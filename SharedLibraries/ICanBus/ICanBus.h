#ifndef ICANBUS_H
#define ICANBUS_H

#include "CanMessage.h"
#include "ECanBitRate.h"

class ICANBus {
public:
    virtual ~ICANBus() = default;

    virtual void Setup(int canTx, int canRx, ECanBitRate bitRate) = 0;

    virtual void SendMessage(CanMessage &message) = 0;

    virtual bool ReceiveAvailable() = 0;

    virtual void Receive() = 0;

    const uint8_t START_BYTE = 0xAA;
};

#endif // ICANBUS_H
