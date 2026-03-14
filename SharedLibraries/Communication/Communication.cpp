#include "Communication.h"
#include "Logger.h"

Communication::Communication(ICANBus &canBus, CANQueue &stringQueue) : nextId(1), canBus(canBus),
                                                                       canQueue(stringQueue) {
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

void Communication::Notify(CANMessage message) {
    for (const auto &subscriber: subscribers) {
        subscriber.callback(message);
    }
}

void Communication::CheckForMessage() {
    if (canBus.ReceiveAvailable()) {
        canBus.Receive();
    }
    CANMessage message;

    if (canQueue.dequeue(message)) {
        Notify(message);
    }
}

template<typename T>
void Communication::SendCANMessage(uint16_t canID, const T &messageStruct) {
    CANMessage msg;
    msg.id = canID;

    // Determine number of bytes to send (struct size or 8 max)
    uint8_t len = sizeof(T);
    if (len > 8) len = 8; // standard CAN max payload

    msg.dlc = len;
    memcpy(msg.data, &messageStruct, len);

    canBus.SendMessage(msg);
}
