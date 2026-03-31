#include "Communication.h"
#include "Logger.h"
#include "CanID.h"
#include "LargeCanMessage.h"
#define MAX_CAN_DATA_LENGTH 8

Communication::Communication(ICANBus &canBus, CanQueue &stringQueue, LargeCanMessageHandler &largeCanMessageHandler,
                             ECanNode me) : nextId(0), canBus(canBus),
                                            canQueue(stringQueue), largeCanMessageHandler(largeCanMessageHandler),
                                            me(me) {
}

Communication::~Communication() {
    subscribers.clear();
}

int Communication::Subscribe(Callback callback) {
    Subscription subscriber;
    subscriber.id = nextId++;
    subscriber.callback = std::move(callback);
    subscribers.push_back(subscriber);
    return subscriber.id;
}

void Communication::Unsubscribe(int id) {
    if (id == -1) {
        return;
    }
    for (int i = 0; i < subscribers.size(); i++) {
        if (subscribers[i].id == id) {
            subscribers.erase(subscribers.begin() + i);
            return;
        }
    }
}

void Communication::Notify(const CanId &canId, const uint8_t *data, uint8_t length) {
    for (const auto &subscriber: subscribers) {
        subscriber.callback(canId, data, length);
    }
}

void Communication::CheckForMessage() {
    if (canBus.ReceiveAvailable()) {
        canBus.Receive();
    }
    CanMessage message{};

    if (canQueue.dequeue(message)) {
        DecodeCanMessage(message);
    }
}

void Communication::DecodeCanMessage(const CanMessage &message) {
    CanId canID;
    if (!canID.FromRaw(message.id)) {
        return;
    }
    if (!canID.IsForNode(me)) {
        return;
    }
    if (canID.HasFlag()) {
        LargeCanMessage *largeMessage = largeCanMessageHandler.HandleLargeCanMessage(message);
        if (largeMessage) {
            Notify(canID, largeMessage->data.data(), largeMessage->length);
            largeCanMessageHandler.RemoveLargeMessage(canID.src, canID.type);
            return;
        }
    }
    Notify(canID, message.data, message.dlc);
}

void Communication::SendCANMessage(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length) {
    CanId canId;
    canId.src = me;
    canId.dst = target;
    canId.type = type;
    canId.flags = ECanFlags::FLAG_NONE;

    if (length > MAX_CAN_DATA_LENGTH) {
        largeCanMessageHandler.SendLargeMessage(canId, data, length);
        return;
    }

    CanMessage msg{};
    msg.id = canId.ToRaw();
    msg.dlc = length;
    memcpy(msg.data, data, length);

    canBus.SendMessage(msg);
}
