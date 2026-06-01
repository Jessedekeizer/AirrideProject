#include "OTACommunication.h"
#include "Logger.h"

OTACommunication::OTACommunication(Communication &communication)
    : communication(communication) {}

void OTACommunication::OnDiscover() {
    LOG_INFO("OTA DISCOVER received, replying AVAILABLE");
    SendStatus(EOTAStatusType::AVAILABLE, EOTAUpdatePhase::IDLE, 0);
}

void OTACommunication::SendStatus(EOTAStatusType type, EOTAUpdatePhase phase, uint8_t progress) {
    CANAirRideOTAStatus status{};
    status.type = type;
    status.phase = phase;
    status.progress = progress;
    communication.SendCanMessage(ECanNode::NODE_AIRRIDE_GUI, ECanMsgType::CAN_AIRRIDE_OTA_STATUS, status);
}
