#ifndef CANBUS_H
#define CANBUS_H
#include "CANQueue.h"
#include "ICANBus.h"

#define CAN1TX 11
#define CAN1RX 12

class CanBus : public ICANBus {
public:
    CanBus(CanQueue &canQueue) : canQueue(canQueue), canReady(false) {
    };

    void Setup(int canTx, int canRx, ECanBitRate bitRate) override;

    void SendMessage(CanMessage &message) override;

    bool ReceiveAvailable() override;

    void Receive() override;

private:
    CanQueue &canQueue;
    bool canReady;

    void SetCan1Pins();
};

#endif //CANBUS_H
