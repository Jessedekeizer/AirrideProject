#ifndef ICANBUS_H
#define ICANBUS_H

#include "CanMessage.h"
#include "ECanBitRate.h"
#include "freertos_include.h"

class ICANBus {
public:
    virtual ~ICANBus() = default;

    virtual void Setup(int canTx, int canRx, ECanBitRate bitRate) = 0;

    virtual QueueHandle_t GetRxQueue() const = 0;
    virtual QueueHandle_t GetTxQueue() const = 0;
};

#endif // ICANBUS_H
