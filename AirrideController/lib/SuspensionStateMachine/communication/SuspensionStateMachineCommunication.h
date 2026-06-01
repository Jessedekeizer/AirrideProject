#ifndef SUSPENSIONSTATEMACHINECOMMUNICATION_H
#define SUSPENSIONSTATEMACHINECOMMUNICATION_H
#include "Communication.h"
#include "SuspensionStateMachineData.h"

class SuspensionStateMachineCommunication {
public:
    SuspensionStateMachineCommunication(Communication &communication, SuspensionStateMachineData &suspensionStateMachineData);

    void Init();
    void Leave();

private:
    void RequestChangeState(const uint8_t *data, uint8_t length);
    void ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length);

    Communication &communication;
    SuspensionStateMachineData &suspensionStateMachineData;
    int communicationId = -1;
};

#endif //SUSPENSIONSTATEMACHINECOMMUNICATION_H
