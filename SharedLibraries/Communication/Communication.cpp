#include "Communication.h"
#include "Logger.h"
#include "CANID.h"
#include "LargeCanMessage.h"

Communication::Communication(ICANBus &canBus, CanQueue &stringQueue, LargeCanMessageHandler &largeCanMessageHandler,
                             ECanNode me) : nextId(1), canBus(canBus),
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
    //LOG_DEBUG("CheckForMessage");
    if (canBus.ReceiveAvailable()) {
        canBus.Receive();
    }
    CanMessage message{};

    if (canQueue.dequeue(message)) {
        DecodeCanMessage(message);
    }
    //TODO add message timeOutFor large messages to be removed when nothing is received
}

void Communication::DecodeCanMessage(const CanMessage &message) {
    CanId canID;
    if (!canID.FromRaw(message.id)) {
        LOG_ERROR("CanID parse failed SENDER: src:", static_cast<int8_t>(canID.src), "dst:",
                  static_cast<int8_t>(canID.dst), "type:", static_cast<int16_t>(canID.type), "flags:",
                  static_cast<int8_t>(canID.flags));
        return;
    }
    if (!canID.IsForNode(me)) {
        return;
    }
    if (canID.flags == ECanFlags::FLAG_NONE) {
        Notify(canID, message.data, message.dlc);
    }
    if (canID.flags >= ECanFlags::FLAG_LARGE_MESSAGE) {
        LargeCanMessage *largeMessage = largeCanMessageHandler.HandleLargeCanMessage(message);
        if (largeMessage) {
            Notify(canID, largeMessage->data.data(), largeMessage->length);
            largeCanMessageHandler.RemoveLargeMessage(canID.src, canID.type);
        }
    }
}

void Communication::SendCANMessage(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length) {
    CanId canId;
    canId.src = me;
    canId.dst = target;
    canId.type = type;
    canId.flags = ECanFlags::FLAG_NONE;

    if (length > 8) {
        largeCanMessageHandler.SendLargeMessage(canId, data, length);
        return;
    }

    CanMessage msg{};
    msg.id = canId.ToRaw();
    msg.dlc = length;
    memcpy(msg.data, data, length);

    canBus.SendMessage(msg);
}
