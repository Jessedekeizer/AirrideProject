#ifndef OTACOMMUNICATION_H
#define OTACOMMUNICATION_H
#include "Communication.h"
#include "CanMessages.h"

class OTACommunication {
public:
    OTACommunication(Communication &communication);

    void OnDiscover();
    void SendStatus(EOTAStatusType type, EOTAUpdatePhase phase, uint8_t progress);

private:
    Communication &communication;
};

#endif // OTACOMMUNICATION_H
