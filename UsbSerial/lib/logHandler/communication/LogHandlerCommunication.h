#ifndef LOGHANDLERCOMMUNICATION_H
#define LOGHANDLERCOMMUNICATION_H
#include "Communication.h"
#include "Arduino.h"

class LogHandlerCommunication {
public:
    LogHandlerCommunication(Communication &communication);

    void SendLog(String &message);

private:
    Communication &communication;
};

#endif //LOGHANDLERCOMMUNICATION_H
