#include "MainScreenCommunication.h"
#include "CanMessageIds.h"
#include "CanMessages.h"
#include "Logger.h"

/**
 * @brief Bind the screen data, log storage and settings to the bus.
 */
MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData,
                                                 LogStorage &logStorage, SettingsDevice &settings)
    : communication(communication), mainScreenData(mainScreenData), logStorage(logStorage), settings(settings),
      communicationId(-1) {
}

/**
 * @brief Start listening to the bus. Safe to call when already subscribed.
 * @note Bound to the main screen being up, so nothing is decoded while the
 *       screen that shows it is not there.
 */
void MainScreenCommunication::Subscribe() {
    if (communicationId != -1) {
        return;
    }
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
    LOG_DEBUG("MainScreenCommunication subscribed", communicationId);
}

/**
 * @brief Stop listening. Safe to call when not subscribed.
 * @note Frames still queue up in Communication; they are simply dropped
 *       undecoded when the queue is next drained.
 */
void MainScreenCommunication::Unsubscribe() {
    if (communicationId == -1) {
        return;
    }
    LOG_DEBUG("MainScreenCommunication unsubscribing", communicationId);
    communication.Unsubscribe(communicationId);
    communicationId = -1;
}

/**
 * @brief Route one decoded frame to its handler.
 * @param canId  Decoded identifier.
 * @param data   Payload.
 * @param length Payload length.
 * @note Runs on the UI thread - Communication::CheckForMessage() is called from
 *       loop() - which is what lets MainScreenData go unlocked.
 */
void MainScreenCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == ECanMsgType::CAN_AIRRIDE_PRESSURE) {
        HandlePressureMessage(data, length);
    }
    if (canId.type == ECanMsgType::CAN_AIRRIDE_LOG) {
        HandleLogMessage(data, length);
    }
    if (canId.type == ECanMsgType::CAN_AIRRIDE_ACK) {
        HandleAck(data, length);
    }
}

/**
 * @brief Store the latest front and back readings.
 * @param data   Payload.
 * @param length Payload length.
 */
void MainScreenCommunication::HandlePressureMessage(const uint8_t *data, uint8_t length) {
    CANAirRidePressure pressure{};
    if (decodeCANMessage(data, length, pressure)) {
        mainScreenData.front = pressure.front;
        mainScreenData.back = pressure.back;
    }
}

/**
 * @brief Append one completed move to the card.
 * @param data   Payload.
 * @param length Payload length.
 * @note The card write blocks this thread, so it costs a frame. Log frames only
 *       arrive when a move finishes.
 */
void MainScreenCommunication::HandleLogMessage(const uint8_t *data, uint8_t length) {
    CANLogAirRide log{};
    if (decodeCANMessage(data, length, log)) {
        LOG_DEBUG("Received log message");
        logStorage.WriteLog(CreateLogMessage(log.front, log.startPressure, log.endPressure, log.startTankPressure,
                                             log.time, log.direction, log.togetherMove));
    }
}

/**
 * @brief Report whether the controller accepted the settings.
 * @param data   Payload.
 * @param length Payload length.
 */
void MainScreenCommunication::HandleAck(const uint8_t *data, uint8_t length) {
    CanAckPayload ack{};
    if (!decodeCANMessage(data, length, ack)) {
        return;
    }
    if (ack.type != ECanMsgType::CAN_AIRRIDE_SETTINGS) {
        return;
    }
    if (ack.status == ECanAckStatus::STATUS_OK) {
        LOG_INFO("Settings ack: controller applied settings");
    } else {
        LOG_ERROR("Settings ack: controller reported error applying settings");
    }
}

/**
 * @brief Push the whole settings struct to the controller, ack required.
 */
void MainScreenCommunication::SendSettings() {
    CANSettingsAirRide canSettingsAirRide{
        settings.frontMax,
        settings.backMax,
        settings.rideFront,
        settings.rideBack,
        settings.frontUpX,
        settings.frontDownX,
        settings.backUpX,
        settings.backDownX,
        settings.parkDuration
    };
    communication.SendCanMessage(ECanNode::NODE_AIRRIDE_CONTROLLER, ECanMsgType::CAN_AIRRIDE_SETTINGS,
                                 canSettingsAirRide, true);
}

/**
 * @brief Send one control frame.
 * @param button Which control was used.
 * @param state  true sets that flag; false clears every flag.
 * @note The buttons are momentary, so a release means stop, not reverse.
 */
void MainScreenCommunication::SendMessageButtonPress(EMainScreenButtons button, bool state) {
    CANAirRideControl canAirRideControl{false, false, false, false, false, false};
    if (state) {
        switch (button) {
            case FRONT_UP: {
                canAirRideControl.frontUp = true;
                break;
            }
            case FRONT_DOWN: {
                canAirRideControl.frontDown = true;
                break;
            }
            case BACK_UP: {
                canAirRideControl.backUp = true;
                break;
            }
            case BACK_DOWN: {
                canAirRideControl.backDown = true;
                break;
            }
            case PARK: {
                canAirRideControl.park = true;
                break;
            }
            case RIDE: {
                canAirRideControl.ride = true;
                break;
            }
            default: {
                return;
            }
        }
    }
    communication.SendCanMessage(ECanNode::NODE_AIRRIDE_CONTROLLER, ECanMsgType::CAN_AIRRIDE_CONTROL,
                                 canAirRideControl);
}

/**
 * @brief Format one move record as the line written to the card.
 * @return The formatted record.
 */
String MainScreenCommunication::CreateLogMessage(bool front, float startPressure, float endPressure,
                                                 float startTankPressure, unsigned long time, bool direction,
                                                 bool togetherMove) {
    String message = front ? "LOGF/" : "LOGB/";
    return message + startPressure + "/" + endPressure + "/" + startTankPressure + "/" + time + "/" + direction + "/" +
           togetherMove + ";";
}
