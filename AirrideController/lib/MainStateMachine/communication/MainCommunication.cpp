#include "MainCommunication.h"
#include "CanMessages.h"
#include "Logger.h"

MainCommunication::MainCommunication(Communication &communication, Settings &settings,
                                     MainStateMachineData &controllerData, OTACommunication &otaCommunication)
    : communication(communication), settings(settings),
      controllerData(controllerData), otaCommunication(otaCommunication) {}

void MainCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
}

void MainCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void MainCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == ECanMsgType::CAN_AIRRIDE_SETTINGS) {
        SaveSettings(data, length);
    } else if (canId.type == ECanMsgType::CAN_AIRRIDE_OTA) {
        HandleOTACommand(data, length);
    }
}

void MainCommunication::HandleOTACommand(const uint8_t *data, uint8_t length) {
    CANAirRideOTA msg{};
    if (!decodeCANMessage(data, length, msg)) {
        LOG_ERROR("Failed to decode OTA message");
        return;
    }

    switch (msg.command) {
        case EOTACommand::DISCOVER:
            otaCommunication.OnDiscover();
            break;
        case EOTACommand::START:
            LOG_INFO("OTA START received");
            controllerData.requestedState = EMainState::OTA;
            break;
        case EOTACommand::STOP:
            LOG_INFO("OTA STOP received");
            controllerData.requestedState = EMainState::SUSPENSION;
            break;
        default:
            LOG_ERROR("MainCommunication: unknown OTA command: ", (uint8_t)msg.command);
            break;
    }
}

void MainCommunication::SaveSettings(const uint8_t *data, uint8_t length) {
    CANSettingsAirRide settingsAirRide{};
    if (!decodeCANMessage(data, length, settingsAirRide)) {
        LOG_ERROR("Failed to decode settings message");
        return;
    }
    settings.backDownX = settingsAirRide.backDownX;
    settings.backUpX = settingsAirRide.backUpX;
    settings.frontDownX = settingsAirRide.frontDownX;
    settings.frontUpX = settingsAirRide.frontUpX;
    settings.frontMax = settingsAirRide.frontMax;
    settings.backMax = settingsAirRide.backMax;
    settings.rideBack = settingsAirRide.rideBack;
    settings.rideFront = settingsAirRide.rideFront;
    settings.parkDuration = settingsAirRide.parkDuration;

    LOG_INFO("backDownX: ", settings.backDownX);
    LOG_INFO("backUpX: ", settings.backUpX);
    LOG_INFO("frontDownX: ", settings.frontDownX);
    LOG_INFO("frontUpX: ", settings.frontUpX);
    LOG_INFO("frontMax: ", settings.frontMax);
    LOG_INFO("backMax: ", settings.backMax);
    LOG_INFO("rideBack: ", settings.rideBack);
    LOG_INFO("rideFront: ", settings.rideFront);
    LOG_INFO("parkDuration: ", settings.parkDuration);

    LOG_INFO("Settings saved");
}
