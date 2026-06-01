#include "SuspensionStateMachineCommunication.h"
#include "CANMessages.h"
#include "EState.h"
#include "Logger.h"

SuspensionStateMachineCommunication::SuspensionStateMachineCommunication(Communication &communication,
                                                                         SuspensionStateMachineData &suspensionStateMachineData)
    : communication(communication), suspensionStateMachineData(suspensionStateMachineData) {}

void SuspensionStateMachineCommunication::Init() {
    communicationId = communication.Subscribe([this](const CanId &canId, const uint8_t *data, uint8_t length) {
        ReceiveCallback(canId, data, length);
    });
}

void SuspensionStateMachineCommunication::Leave() {
    communication.Unsubscribe(communicationId);
}

void SuspensionStateMachineCommunication::ReceiveCallback(const CanId &canId, const uint8_t *data, uint8_t length) {
    if (canId.type == ECanMsgType::CAN_AIRRIDE_CONTROL) {
        RequestChangeState(data, length);
    }
}

void SuspensionStateMachineCommunication::RequestChangeState(const uint8_t *data, uint8_t length) {
    CANAirRideControl canAirRideControl{};
    if (!decodeCANMessage(data, length, canAirRideControl)) {
        LOG_ERROR("Failed to decode settings message");
        return;
    }
    if (canAirRideControl.frontUp) {
        suspensionStateMachineData.newRequestedState = EState::FRONT_UP;
    } else if (canAirRideControl.backUp) {
        suspensionStateMachineData.newRequestedState = EState::BACK_UP;
    } else if (canAirRideControl.frontDown) {
        suspensionStateMachineData.newRequestedState = EState::FRONT_DOWN;
    } else if (canAirRideControl.backDown) {
        suspensionStateMachineData.newRequestedState = EState::BACK_DOWN;
    } else if (canAirRideControl.ride) {
        suspensionStateMachineData.newRequestedState = EState::RIDE;
    } else if (canAirRideControl.park) {
        suspensionStateMachineData.newRequestedState = EState::PARK;
    } else {
        suspensionStateMachineData.newRequestedState = EState::IDLE;
    }
    LOG_DEBUG("newState", static_cast<int>(suspensionStateMachineData.newRequestedState));
}
