#ifndef CANBUS_H
#define CANBUS_H
#include "CanMessage.h"
#include "ECanBitRate.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/twai.h"

#define CAN_RX_QUEUE_SIZE 20
#define CAN_TX_QUEUE_SIZE 20

class CanBus {
public:
    CanBus() : canReady(false), rxQueue(nullptr), txQueue(nullptr) {};

    void Setup(int canTx, int canRx, ECanBitRate bitRate);

    QueueHandle_t GetRxQueue() const { return rxQueue; }
    QueueHandle_t GetTxQueue() const { return txQueue; }

private:
    bool canReady;
    QueueHandle_t rxQueue;
    QueueHandle_t txQueue;

    static void RxTask(void *arg);
    static void TxTask(void *arg);
};


#endif //CANBUS_H
