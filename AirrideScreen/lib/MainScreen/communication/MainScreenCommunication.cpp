#include "MainScreenCommunication.h"
#include "CANMessages.h"
#include "CANMessageIDs.h"
#include "Logger.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData,
                                                 LogStorage &logStorage)
    : communication(communication), mainScreenData(mainScreenData), logStorage(logStorage), communicationId(-1) {
}

void MainScreenCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) { ReceiveCallback(canId, data, length); });
}

void MainScreenCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == CanMsgType::CAN_AIRRIDE_PRESSURE) {
        HandlePressureMessage(data, length);
    }
    //TODO add log handling

    // if (message.startsWith("LOG")) {
    //     int semiColonIndex = message.indexOf(";");
    //     logStorage.WriteLog(message.substring(0, semiColonIndex + 1));
    // }
}

void MainScreenCommunication::HandlePressureMessage(const uint8_t *data, uint8_t length) {
    CANAirRidePressure pressure = decodeCANMessage<CANAirRidePressure>(data, length);
    mainScreenData.front = pressure.front;
    mainScreenData.back = pressure.back;
}

void MainScreenCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

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
    communication.SendCANMessage(CanNode::NODE_R4, CanMsgType::CAN_AIRRIDE_CONTROL, canAirRideControl);
}
