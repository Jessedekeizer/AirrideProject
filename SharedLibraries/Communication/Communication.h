#ifndef COMMUNICATION_H
#define COMMUNICATION_H
#include <functional>
#include <vector>
#include "ICANBus.h"
#include "CANQueue.h"

using Callback = std::function<void(CANMessage &)>;

class Communication {
public:
    Communication(ICANBus &canBus, CANQueue &stringQueue);

    ~Communication();

    int Subscribe(Callback callback);

    void Unsubscribe(int id);

    void Notify(CANMessage message);

    void CheckForMessage();

    template<typename T>
    void SendCANMessage(uint16_t canID, const T &messageStruct);

private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };

    std::vector<Subscription> subscribers;
    unsigned int nextId;
    ICANBus &canBus;
    CANQueue &canQueue;
};

#endif //COMMUNICATIONDISTRIBUTOR_H
