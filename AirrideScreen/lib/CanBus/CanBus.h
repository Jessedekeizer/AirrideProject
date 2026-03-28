#ifndef CANBUS_H
#define CANBUS_H
#include "CANQueue.h"
#include "ICANBus.h"


class CanBus : public ICANBus {
public:
    CanBus(CANQueue &canQueue) : canQueue(canQueue), canReady(false) {
    };

    void Setup(int canTx, int canRx, ECanBitRate bitRate) override;

    void SendMessage(CanMessage &message) override;

    bool ReceiveAvailable() override;

    void Receive() override;

private:
    CANQueue &canQueue;
    bool canReady;
};


#endif //CANBUS_H
