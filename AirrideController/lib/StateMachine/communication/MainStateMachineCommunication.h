#ifndef MAINSTATEMACHINECOMMUNICATION_H
#define MAINSTATEMACHINECOMMUNICATION_H
#include "Communication.h"
#include "MainStateMachineData.h"

class MainStateMachineCommunication {
public:
    MainStateMachineCommunication(Communication &communication, MainStateMachineData &mainStateMachineData);

    void Init();

    void Leave();

private:
    void RequestChangeState(const uint8_t *data, uint8_t length);


    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    Communication &communication;
    MainStateMachineData &mainStateMachineData;
    int communicationId;
};

#endif //MAINSTATEMACHINECOMMUNICATION_H
