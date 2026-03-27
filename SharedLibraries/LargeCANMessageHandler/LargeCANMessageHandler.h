#ifndef LARGECANMESSAGEHANDLER_H
#define LARGECANMESSAGEHANDLER_H
#include <vector>
#include "LargeCANMessage.h"
#include "CanMessage.h"
#include "ICANBus.h"

class LargeCanMessageHandler {
public:
    LargeCanMessageHandler(ICANBus &canBus) : canBus(canBus) {
    };

    ~LargeCanMessageHandler() {
    };
    LargeCanMessage *HandleLargeCanMessage(const CanMessage &message);

    void SendLargeMessage(CanId &canId, const uint8_t *data, uint8_t length);
    void RemoveLargeMessage(CanNode sender, CanMsgType msgType);

private:
    LargeCanMessage *GetLargeCanMessage(CanNode sender, CanMsgType msgType);
    void AppendMessageToLargeMessage(LargeCanMessage &largeCanMessage, const CanMessage &message);
    std::vector<LargeCanMessage> largeCANMessages{};
    ICANBus &canBus;
};

#endif // LARGECANMESSAGEHANDLER_H
