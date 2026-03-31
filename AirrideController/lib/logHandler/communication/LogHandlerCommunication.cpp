#include "LogHandlerCommunication.h"
#include "CANMessages.h"

LogHandlerCommunication::LogHandlerCommunication(Communication &communication) : communication(communication) {
}

void LogHandlerCommunication::SendLog(bool front, float startPressure, float endPressure, float startTankPressure,
                                      unsigned long time, bool direction, bool togetherMove) {
    CANLogAirRide canLogAirRide{
        front, startPressure, endPressure, startTankPressure, static_cast<uint16_t>(time), direction, togetherMove
    };
    communication.SendCanMessage(ECanNode::NODE_AIRRIDE_GUI, ECanMsgType::CAN_AIRRIDE_LOG, canLogAirRide);
}



