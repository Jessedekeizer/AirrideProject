#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <functional>
#include <vector>
#include "CANID.h"
#include "ICANBus.h"
#include "CANQueue.h"
#include "LargeCANMessageHandler.h"

using Callback = std::function<void(const CanId &, const uint8_t *, uint8_t)>;

class Communication {
public:
    Communication(ICANBus &canBus, CANQueue &stringQueue, LargeCanMessageHandler &largeCanMessageHandler, CanNode me);

    ~Communication();

    int Subscribe(Callback callback);

    void Unsubscribe(int id);

    void Notify(const CanId &canId, const uint8_t *data, uint8_t length);

    void CheckForMessage();

    template<typename T>
    void SendCANMessage(CanNode target, CanMsgType type, const T &messageStruct) {
        SendCANMessage(target, type,
                       reinterpret_cast<const uint8_t *>(&messageStruct),
                       static_cast<uint8_t>(sizeof(T)));
    }

private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };

    void SendCANMessage(CanNode target, CanMsgType type, const uint8_t *data, uint8_t length);

    void DecodeCanMessage(const CanMessage &message);

    std::vector<Subscription> subscribers;
    unsigned int nextId;
    ICANBus &canBus;
    CANQueue &canQueue;
    LargeCanMessageHandler &largeCanMessageHandler;
    const CanNode me;
};

#endif // COMMUNICATION_H
