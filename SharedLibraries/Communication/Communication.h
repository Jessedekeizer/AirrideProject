#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <functional>
#include <WString.h>
#include <vector>
#include "ISerial.h"
#include "StringQueue.h"

using Callback = std::function<void(String &)>;

class Communication {
public:
    Communication(ISerial &serial, StringQueue &stringQueue);
    ~Communication();
    int Subscribe(Callback callback);
    void Unsubscribe(int id);
    void Notify(String &message);
    void CheckForMessage();
    void SendMessage(String &message);

private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };

    std::vector<Subscription> subscribers;
    unsigned int nextId;
    ISerial &serial;
    StringQueue &stringQueue;
};

#endif //COMMUNICATIONDISTRIBUTOR_H
