#ifndef LOGHANDLERCOMMUNICATION_H
#define LOGHANDLERCOMMUNICATION_H
#include "Communication.h"
#include "Arduino.h"

class LogHandlerCommunication {
public:
    LogHandlerCommunication(Communication &communication);

    void SendLog(bool front, float startPressure, float endPressure, float startTankPressure,
                 unsigned long time, bool direction, bool togetherMove);

private:
    Communication &communication;
};

#endif //LOGHANDLERCOMMUNICATION_H
