#include "MainScreenCommunication.h"
#include "CanMessages.h"
#include "CanMessageIds.h"
#include "Logger.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData,
                                                 LogStorage &logStorage)
    : communication(communication), mainScreenData(mainScreenData), logStorage(logStorage), communicationId(-1) {
}

void MainScreenCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
}

void MainScreenCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == ECanMsgType::CAN_AIRRIDE_PRESSURE) {
        HandlePressureMessage(data, length);
    }
    if (canId.type == ECanMsgType::CAN_AIRRIDE_LOG) {
        HandleLogMessage(data, length);
    }
}

void MainScreenCommunication::HandlePressureMessage(const uint8_t *data, uint8_t length) {
    CANAirRidePressure pressure{};
    if (decodeCANMessage(data, length, pressure)) {
        mainScreenData.front = pressure.front;
        mainScreenData.back = pressure.back;
    }
}

void MainScreenCommunication::HandleLogMessage(const uint8_t *data, uint8_t length) {
    CANLogAirRide log{};
    if (decodeCANMessage(data, length, log)) {
        LOG_DEBUG("Received log message");
        logStorage.WriteLog(CreateLogMessage(log.front, log.startPressure, log.endPressure, log.startTankPressure,
                                             log.time, log.direction, log.togetherMove));
    }
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
    communication.SendCANMessage(ECanNode::NODE_AIRRIDE_CONTROLLER, ECanMsgType::CAN_AIRRIDE_CONTROL, canAirRideControl);
}

String MainScreenCommunication::CreateLogMessage(bool front, float startPressure, float endPressure,
                                                 float startTankPressure,
                                                 unsigned long time, bool direction, bool togetherMove) {
    String message = front ? "LOGF/" : "LOGB/";
    return message + startPressure + "/" + endPressure + "/" + startTankPressure + "/" + time + "/" + direction + "/" +
           togetherMove + ";";
}
