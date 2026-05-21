#ifndef LARGECANMESSAGEHANDLER_H
#define LARGECANMESSAGEHANDLER_H
#include <vector>
#include "LargeCanMessage.h"
#include "CanMessage.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

class LargeCanMessageHandler {
public:
    LargeCanMessageHandler() : txQueue(nullptr) {};

    ~LargeCanMessageHandler() {};

    void SetTxQueue(QueueHandle_t queue) { txQueue = queue; }

    LargeCanMessage *HandleLargeCanMessage(const CanMessage &message);

    void SendLargeMessage(CanId &canId, const uint8_t *data, uint8_t length);

    void RemoveLargeMessage(ECanNode sender, ECanMsgType msgType);

private:
    LargeCanMessage *GetLargeCanMessage(ECanNode sender, ECanMsgType msgType);

    void AppendMessageToLargeMessage(LargeCanMessage *largeCanMessage, const CanMessage &message);

    void CreateNewLargeMessage(CanId &canId, const CanMessage &message);

    std::vector<LargeCanMessage> largeCANMessages{};
    QueueHandle_t txQueue;
};

#endif // LARGECANMESSAGEHANDLER_H
