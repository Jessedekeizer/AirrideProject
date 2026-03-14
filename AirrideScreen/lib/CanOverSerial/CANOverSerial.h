#ifndef SERIALOVERPINS_H
#define SERIALOVERPINS_H
#include <HardwareSerial.h>
#include "CANQueue.h"
#include "ICANBus.h"

class CANOverSerial : public ICANBus {
public:
    CANOverSerial(HardwareSerial &serial, CANQueue &canQueue) : serial(serial), canQueue(canQueue), message({0}) {
    };

    void SendMessage(CANMessage &message) override;

    bool ReceiveAvailable() override;

    void Receive() override;

private:
    HardwareSerial &serial;
    CANQueue &canQueue;
    CANMessage message;
};

#endif //SERIALOVERPINS_H
