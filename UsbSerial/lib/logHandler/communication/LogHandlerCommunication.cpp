#include "LogHandlerCommunication.h"

LogHandlerCommunication::LogHandlerCommunication(Communication &communication) : communication(communication) {
}

void LogHandlerCommunication::SendLog(String &message) {
    //communication.SendMessage(message);
}

