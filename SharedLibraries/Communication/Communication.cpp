#include "Communication.h"

#include "Logger.h"

Communication::Communication(ISerial &serial, StringQueue &stringQueue) : nextId(1), serial(serial),
                                                                          stringQueue(stringQueue) {
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

void Communication::Notify(String &message) {
    for (const auto &subscriber: subscribers) {
        subscriber.callback(message);
    }
}

void Communication::CheckForMessage() {
    serial.Receive();
    String message;

    if (stringQueue.dequeue(message)) {
        if (message.length() > 0) {
            LOG_DEBUG(message);
            Notify(message);
        }
    }
}

void Communication::SendMessage(String &message) {
    LOG_DEBUG(message);
    serial.SendMessage(message);
}
