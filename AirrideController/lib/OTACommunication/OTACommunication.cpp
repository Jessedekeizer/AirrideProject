#include "OTACommunication.h"
#include "Logger.h"
#include <Arduino.h>

OTACommunication::OTACommunication(Communication &communication)
    : communication(communication) {}

void OTACommunication::Init() {
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

void OTACommunication::Loop() {
    if (!updating) return;

    unsigned long elapsed = millis() - updateStartMs;
    uint8_t progress = (elapsed >= FAKE_DURATION_MS)
        ? 100
        : (uint8_t)(elapsed * 100UL / FAKE_DURATION_MS);

    if (!startSent) {
        startSent = true;
        SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::FLASHING, 0);
    }

    if (progress != lastSentProgress) {
        lastSentProgress = progress;

        if (failAtProgress != 255 && progress >= failAtProgress) {
            SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::ERROR, progress);
            updating = false;
            LOG_INFO("OTA fake update failed at ", progress, "%");
            return;
        }

        bool done = (progress >= 100);
        SendStatus(EOTAStatusType::UPDATE, done ? EOTAUpdatePhase::COMPLETE : EOTAUpdatePhase::FLASHING, progress);
        if (done) {
            updating = false;
            LOG_INFO("OTA fake update complete");
        }
    }
}

void OTACommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type != ECanMsgType::CAN_AIRRIDE_OTA) return;

    CANAirRideOTA msg{};
    if (!decodeCANMessage(data, length, msg)) return;

    switch (msg.command) {
        case EOTACommand::DISCOVER:
            LOG_INFO("OTA DISCOVER received, replying AVAILABLE");
            SendStatus(EOTAStatusType::AVAILABLE, EOTAUpdatePhase::IDLE, 0);
            break;

        case EOTACommand::START:
            LOG_INFO("OTA START received, beginning fake update");
            updating = true;
            startSent = false;
            lastSentProgress = 255;
            updateStartMs = millis();
            failAtProgress = (random(4) == 0) ? (uint8_t)(20 + random(60)) : 255;
            break;

        case EOTACommand::STOP:
            LOG_INFO("OTA STOP received");
            updating = false;
            break;
        default:
            LOG_ERROR("OTACommunication: unknown command: ", (uint8_t)msg.command);
            break;
    }
}

void OTACommunication::SendStatus(EOTAStatusType type, EOTAUpdatePhase phase, uint8_t progress) {
    CANAirRideOTAStatus status{};
    status.type = type;
    status.phase = phase;
    status.progress = progress;
    communication.SendCanMessage(ECanNode::NODE_AIRRIDE_GUI, ECanMsgType::CAN_AIRRIDE_OTA_STATUS, status);
}
