#include "OTACommunication.h"
#include "Logger.h"

OTACommunication::OTACommunication(Communication &communication)
    : communication(communication) {}

void OTACommunication::Init(StatusCallback callback) {
    onStatus = callback;
    selfOta.Init(callback);
    subscriptionId = communication.Subscribe(
        [this](const CanId &id, const uint8_t *data, uint8_t len) {
            ReceiveCallback(id, data, len);
        });
}

void OTACommunication::Leave() {
    if (subscriptionId >= 0) {
        communication.Unsubscribe(subscriptionId);
        subscriptionId = -1;
    }
}

void OTACommunication::Handle() {
    selfOta.Handle();
}

void OTACommunication::SendDiscover() {
    CANAirRideOTA msg{EOTACommand::DISCOVER};
    communication.SendCanMessage(ECanNode::NODE_BROADCAST, ECanMsgType::CAN_AIRRIDE_OTA, msg);
    LOG_DEBUG("OTACommunication: broadcast DISCOVER");
}

void OTACommunication::SendStart(ECanNode target) {
    if (target == ECanNode::NODE_AIRRIDE_GUI) {
        selfOta.Start();
        return;
    }
    CANAirRideOTA msg{EOTACommand::START};
    communication.SendCanMessage(target, ECanMsgType::CAN_AIRRIDE_OTA, msg);
    LOG_DEBUG("OTACommunication: send START to node ", (int)target);
}

void OTACommunication::SendStop(ECanNode target) {
    if (selfOta.IsActive()) {
        selfOta.Stop();
        return;
    }
    CANAirRideOTA msg{EOTACommand::STOP};
    communication.SendCanMessage(target, ECanMsgType::CAN_AIRRIDE_OTA, msg);
    LOG_DEBUG("OTACommunication: send STOP to node ", (int)target);
}

void OTACommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type != ECanMsgType::CAN_AIRRIDE_OTA_STATUS) return;
    CANAirRideOTAStatus status{};
    if (!decodeCANMessage(data, length, status)) return;
    if ((uint8_t)status.type < (uint8_t)EOTAStatusType::first || (uint8_t)status.type > (uint8_t)EOTAStatusType::last) {
        LOG_ERROR("OTACommunication: unknown status type: ", (uint8_t)status.type);
        status.type = EOTAStatusType::UNKNOWN;
        return;
    }
    if ((uint8_t)status.phase < (uint8_t)EOTAUpdatePhase::first || (uint8_t)status.phase > (uint8_t)EOTAUpdatePhase::last) {
        LOG_ERROR("OTACommunication: unknown update phase: ", (uint8_t)status.phase);
        status.phase = EOTAUpdatePhase::UNKNOWN;
        return;
    }
    if (onStatus) onStatus(canId.src, status);
}
