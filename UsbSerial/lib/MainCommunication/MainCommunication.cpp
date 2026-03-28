#include "MainCommunication.h"
#include "CanMessages.h"
#include "Logger.h"

MainCommunication::MainCommunication(Communication &communication, Settings &settings)
    : communication(communication), settings(settings), communicationId(-1) {
}

void MainCommunication::SendPressure(float front, float back) {
    CANAirRidePressure canAirRidePressure{front, back};
    communication.SendCANMessage(CanNode::NODE_ESP32, CanMsgType::CAN_AIRRIDE_PRESSURE, canAirRidePressure);
}

void MainCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
}

void MainCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void MainCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    LOG_DEBUG("Received data", static_cast<int>(canId.type));
    if (canId.type == CanMsgType::CAN_AIRRIDE_SETTINGS) {
        SaveSettings(data, length);
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
