#ifndef CANBUS_H
#define CANBUS_H
#include "ICANBus.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/twai.h"

#define CAN_RX_QUEUE_SIZE 20
#define CAN_TX_QUEUE_SIZE 20

#define CAN_CORE 0

class CanBus : public ICANBus {
public:
    CanBus() : canReady(false), rxQueue(nullptr), txQueue(nullptr),
               txPin(-1), rxPin(-1), bitRate(ECanBitRate::B500k), setupDone(nullptr),
               droppedFrames(0), lastDropLogMs(0), rxTask(nullptr), txTask(nullptr) {};

    void Setup(int canTx, int canRx, ECanBitRate bitRate) override;

    QueueHandle_t GetRxQueue() const override { return rxQueue; }
    QueueHandle_t GetTxQueue() const override { return txQueue; }

    void Stop();

private:
    bool canReady;
    QueueHandle_t rxQueue;
    QueueHandle_t txQueue;

    int txPin;
    int rxPin;
    ECanBitRate bitRate;

    SemaphoreHandle_t setupDone;

    uint32_t droppedFrames;
    uint32_t lastDropLogMs;

    TaskHandle_t rxTask;
    TaskHandle_t txTask;

    static void InstallTask(void *arg);

    bool InstallDriver();

    static void RxTask(void *arg);
    static void TxTask(void *arg);
};

#endif
