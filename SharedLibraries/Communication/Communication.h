#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <functional>
#include <vector>
#include "CanID.h"
#include "ICanBus.h"
#include "CanQueue.h"
#include "LargeCanMessageHandler.h"

using Callback = std::function<void(const CanId &, const uint8_t *, uint8_t)>;

class Communication {
public:
    Communication(ICANBus &canBus, CanQueue &stringQueue, LargeCanMessageHandler &largeCanMessageHandler, ECanNode me);

    ~Communication();

    int Subscribe(Callback callback);

    void Unsubscribe(int id);

    void Notify(const CanId &canId, const uint8_t *data, uint8_t length);

    void CheckForMessage();

    template<typename T>
    void SendCANMessage(ECanNode target, ECanMsgType type, const T &messageStruct) {
        SendCANMessage(target, type,
                       reinterpret_cast<const uint8_t *>(&messageStruct),
                       static_cast<uint8_t>(sizeof(T)));
    }

private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };

    void SendCANMessage(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length);

    void DecodeCanMessage(const CanMessage &message);

    std::vector<Subscription> subscribers;
    unsigned int nextId;
    ICANBus &canBus;
    CanQueue &canQueue;
    LargeCanMessageHandler &largeCanMessageHandler;
    const ECanNode me;
};

#endif // COMMUNICATION_H
