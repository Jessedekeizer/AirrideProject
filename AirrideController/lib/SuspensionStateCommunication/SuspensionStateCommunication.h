#ifndef SUSPENSIONSTATECOMMUNICATION_H
#define SUSPENSIONSTATECOMMUNICATION_H
#include "Communication.h"

class SuspensionStateCommunication {
public:
    SuspensionStateCommunication(Communication &communication);

    void SendPressure(float front, float back);

private:
    Communication &communication;
};

#endif //SUSPENSIONSTATECOMMUNICATION_H
