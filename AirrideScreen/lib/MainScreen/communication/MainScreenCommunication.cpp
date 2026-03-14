#include "MainScreenCommunication.h"
#include "CANMessages.h"
#include "CANMessageIDs.h"
#include "Logger.h"

MainScreenCommunication::MainScreenCommunication(Communication &communication, MainScreenData &mainScreenData,
                                                 LogStorage &logStorage)
    : communication(communication), mainScreenData(mainScreenData), logStorage(logStorage), communicationId(-1) {
}

void MainScreenCommunication::Init() {
    communicationId = communication.Subscribe([this](CANMessage &message) { ReceiveCallback(message); });
}


void MainScreenCommunication::ReceiveCallback(CANMessage &message) {
    if (message.id == static_cast<uint16_t>(CAN_ID::CANAirRidePressure)) {
        HandlePressureMessage(message);
    }
    if (message.startsWith("LOG")) {
        int semiColonIndex = message.indexOf(";");
        logStorage.WriteLog(message.substring(0, semiColonIndex + 1));
    }
}

void MainScreenCommunication::HandlePressureMessage(CANMessage &message) {
    CANAirRidePressure pressure = decodeCANMessage<CANAirRidePressure>(message);
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
    communication.SendCANMessage(static_cast<uint16_t>(CAN_ID::CANAirRideControl), canAirRideControl);
}
