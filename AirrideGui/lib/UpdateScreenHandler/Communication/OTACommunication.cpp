#include "OTACommunication.h"

#include "Logger.h"

/**
 * @brief Bind to the bus.
 */
OTACommunication::OTACommunication(Communication &communication)
    : communication(communication) {
}

/**
 * @brief Route status messages to onStatus.
 * @param callback Receives every status, local access point or remote node.
 * @note Call once from setup(). SelfOTA is pumped every loop whatever screen
 *       is up, so it is wired here and stays wired; only the bus half comes
 *       and goes with the update screens.
 */
void OTACommunication::Init(StatusCallback callback) {
    onStatus = callback;
    selfOta.Init(callback);
}

/**
 * @brief Start listening to the bus. Safe to call when already subscribed.
 */
void OTACommunication::Subscribe() {
    if (subscriptionId >= 0) {
        return;
    }
    subscriptionId = communication.Subscribe(
        [this](const CanId &id, const uint8_t *data, uint8_t len) {
            ReceiveCallback(id, data, len);
        });
    LOG_DEBUG("OTACommunication subscribed", subscriptionId);
}

/**
 * @brief Stop listening. Safe to call when not subscribed.
 */
void OTACommunication::Unsubscribe() {
    if (subscriptionId < 0) {
        return;
    }
    LOG_DEBUG("OTACommunication unsubscribing", subscriptionId);
    communication.Unsubscribe(subscriptionId);
    subscriptionId = -1;
}

/**
 * @brief Pump a self update.
 * @note Sets the pumping flag for its duration. A status produced under it is
 *       safe to repaint the screen from, because nothing else is on the stack;
 *       one produced anywhere else is not.
 */
void OTACommunication::Handle() {
    pumping = true;
    selfOta.Handle();
    pumping = false;
}

/**
 * @brief Ask every node that can be updated to say so.
 */
void OTACommunication::SendDiscover() {
    CANAirRideOTA msg{EOTACommand::DISCOVER};
    communication.SendCanMessage(ECanNode::NODE_BROADCAST, ECanMsgType::CAN_AIRRIDE_OTA, msg);
    LOG_DEBUG("OTACommunication: broadcast DISCOVER");
}

/**
 * @brief Start an update on one node, or on this display.
 * @param target Node to update.
 * @note Aimed at this display, nothing reaches the bus: the prepare callback
 *       runs and SelfOTA brings up a WiFi access point instead.
 */
void OTACommunication::SendStart(ECanNode target) {
    if (target == ECanNode::NODE_AIRRIDE_GUI) {
        if (prepareLocal) {
            prepareLocal();
        }
        selfOta.Start();
        return;
    }
    CANAirRideOTA msg{EOTACommand::START};
    communication.SendCanMessage(target, ECanMsgType::CAN_AIRRIDE_OTA, msg, true);
    LOG_DEBUG("OTACommunication: send START to node ", (int) target);
}

/**
 * @brief Abandon whatever is running.
 * @param target Node to stop.
 * @warning Stopping a local update reboots: the memory given up to start the
 *          access point is not restored.
 */
void OTACommunication::SendStop(ECanNode target) {
    if (selfOta.IsActive()) {
        selfOta.Stop();

        LOG_INFO("Self update abandoned, restarting");
        Serial.flush();
        ESP.restart();
        return;
    }
    CANAirRideOTA msg{EOTACommand::STOP};
    communication.SendCanMessage(target, ECanMsgType::CAN_AIRRIDE_OTA, msg);
    LOG_DEBUG("OTACommunication: send STOP to node ", (int) target);
}

/**
 * @brief Decode one frame and pass on anything that is a valid status.
 * @param canId  Decoded identifier.
 * @param data   Payload.
 * @param length Payload length.
 */
void OTACommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == ECanMsgType::CAN_AIRRIDE_ACK) {
        HandleAck(data, length);
        return;
    }
    if (canId.type != ECanMsgType::CAN_AIRRIDE_OTA_STATUS) {
        return;
    }

    CANAirRideOTAStatus status{};
    if (!decodeCANMessage(data, length, status)) {
        return;
    }
    if ((uint8_t) status.type < (uint8_t) EOTAStatusType::first ||
        (uint8_t) status.type > (uint8_t) EOTAStatusType::last) {
        LOG_ERROR("OTACommunication: unknown status type: ", (uint8_t) status.type);
        return;
    }
    if ((uint8_t) status.phase < (uint8_t) EOTAUpdatePhase::first ||
        (uint8_t) status.phase > (uint8_t) EOTAUpdatePhase::last) {
        LOG_ERROR("OTACommunication: unknown update phase: ", (uint8_t) status.phase);
        return;
    }

    if (onStatus) {
        onStatus(canId.src, status);
    }
}

/**
 * @brief Turn an un-acked start into an error the screen can show.
 * @param data   Payload.
 * @param length Payload length.
 */
void OTACommunication::HandleAck(const uint8_t *data, uint8_t length) {
    CanAckPayload ack{};
    if (!decodeCANMessage(data, length, ack)) {
        return;
    }
    if (ack.type != ECanMsgType::CAN_AIRRIDE_OTA) {
        return;
    }

    if (ack.status == ECanAckStatus::STATUS_OK) {
        LOG_INFO("OTACommunication: START acked");
        return;
    }

    LOG_ERROR("OTACommunication: START not acked, giving up");
    if (onStatus) {
        CANAirRideOTAStatus status{EOTAStatusType::UPDATE, EOTAUpdatePhase::ERROR, 0};
        onStatus(ECanNode::UNKNOWN, status);
    }
}
