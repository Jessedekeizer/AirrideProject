#include "MainStateMachine.h"
#include "SuspensionState.h"
#include "OTAState.h"
#include "Logger.h"

MainStateMachine::MainStateMachine(MainStateMachineData &data,
                                                       SuspensionStateMachine &suspensionStateMachine,
                                                       LogHandler &logHandler,
                                                       PressureSensorManager &pressureSensorManager,
                                                       SuspensionStateCommunication &suspensionStateCommunication,
                                                       SolenoidManager &solenoidManager,
                                                       OTACommunication &otaCommunication)
    : data(data),
      suspensionStateMachine(suspensionStateMachine),
      logHandler(logHandler),
      pressureSensorManager(pressureSensorManager),
      suspensionStateCommunication(suspensionStateCommunication),
      solenoidManager(solenoidManager),
      otaCommunication(otaCommunication) {}

MainStateMachine::~MainStateMachine() {
    if (currentState) {
        currentState->Leave();
        delete currentState;
        currentState = nullptr;
    }
}

void MainStateMachine::Begin() {
    data.requestedState = EMainState::SUSPENSION;
    ChangeState(data.requestedState);
}

void MainStateMachine::Loop() {
    if (!currentState) return;

    EMainState stateFromLoop = currentState->Loop();

    if (currentState->GetState() != data.requestedState) {
        ChangeState(data.requestedState);
        return;
    }
    if (currentState->GetState() != stateFromLoop) {
        ChangeState(stateFromLoop);
    }
}

void MainStateMachine::ChangeState(EMainState newState) {
    if (currentState) {
        currentState->Leave();
        delete currentState;
        currentState = nullptr;
    }

    LOG_DEBUG("Main changing state to:", static_cast<int>(newState));

    switch (newState) {
        case EMainState::SUSPENSION:
            currentState = new SuspensionState(suspensionStateMachine, logHandler, pressureSensorManager,
                                                suspensionStateCommunication);
            break;
        case EMainState::OTA:
            currentState = new OTAState(solenoidManager, otaCommunication);
            break;
    }

    data.requestedState = newState;
    currentState->Enter();
}
