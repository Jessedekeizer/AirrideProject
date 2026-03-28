#include "MainStateMachineCommunication.h"
#include "CANMessages.h"
#include "EState.h"
#include "Logger.h"

MainStateMachineCommunication::MainStateMachineCommunication(Communication &communication,
                                                             MainStateMachineData &
                                                             mainStateMachineData)
    : communication(communication), mainStateMachineData(mainStateMachineData), communicationId(-1) {
}

void MainStateMachineCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
}

void MainStateMachineCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void MainStateMachineCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == CanMsgType::CAN_AIRRIDE_CONTROL) {
        RequestChangeState(data, length);
    }
}

void MainStateMachineCommunication::RequestChangeState(const uint8_t *data, uint8_t length) {
    CANAirRideControl canAirRideControl{};
    if (!decodeCANMessage(data, length, canAirRideControl)) {
        LOG_ERROR("Failed to decode settings message");
        return;
    }
    if (canAirRideControl.frontUp) {
        mainStateMachineData.newRequestedState = EState::FRONT_UP;
    } else if (canAirRideControl.backUp) {
        mainStateMachineData.newRequestedState = EState::BACK_UP;
    } else if (canAirRideControl.frontDown) {
        mainStateMachineData.newRequestedState = EState::FRONT_DOWN;
    } else if (canAirRideControl.backDown) {
        mainStateMachineData.newRequestedState = EState::BACK_DOWN;
    } else if (canAirRideControl.ride) {
        mainStateMachineData.newRequestedState = EState::RIDE;
    } else if (canAirRideControl.park) {
        mainStateMachineData.newRequestedState = EState::PARK;
    } else {
        mainStateMachineData.newRequestedState = EState::IDLE;
    }
    LOG_DEBUG("newState", static_cast<int>(mainStateMachineData.newRequestedState));
}
