#include "SuspensionStateCommunication.h"
#include "CanMessages.h"

SuspensionStateCommunication::SuspensionStateCommunication(Communication &communication)
    : communication(communication) {}

void SuspensionStateCommunication::SendPressure(float front, float back) {
    CANAirRidePressure msg{front, back};
    communication.SendCanMessage(ECanNode::NODE_BROADCAST, ECanMsgType::CAN_AIRRIDE_PRESSURE, msg);
}
