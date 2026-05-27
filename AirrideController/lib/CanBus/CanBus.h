#ifndef CANBUS_H
#define CANBUS_H
#include "ICanBus.h"

#define CAN1TX 11
#define CAN1RX 12
#define CAN_RX_QUEUE_SIZE 20
#define CAN_TX_QUEUE_SIZE 20

class CanBus : public ICANBus {
public:
    CanBus() : canReady(false), rxQueue(nullptr), txQueue(nullptr) {}

    void Setup(int canTx, int canRx, ECanBitRate bitRate) override;

    QueueHandle_t GetRxQueue() const override { return rxQueue; }
    QueueHandle_t GetTxQueue() const override { return txQueue; }

private:
    bool canReady;
    QueueHandle_t rxQueue;
    QueueHandle_t txQueue;

    void SetCan1Pins();

    static void RxTask(void *arg);
    static void TxTask(void *arg);
};

#endif //CANBUS_H
