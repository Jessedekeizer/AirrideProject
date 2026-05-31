#include "SuspensionStateMachine.h"
#include "BackDownState.h"
#include "BackUpState.h"
#include "FrontDownState.h"
#include "FrontUpState.h"
#include "IdleState.h"
#include "ParkState.h"
#include "RideState.h"
#include "Logger.h"

SuspensionStateMachine::SuspensionStateMachine(SuspensionStateMachineData &suspensionStateMachineData,
                                               SuspensionStateMachineCommunication &suspensionStateMachineCommunication,
                                               SolenoidManager &solenoidManager,
                                               PressureSensorManager &pressureSensorManager,
                                               LogHandler &logHandler,
                                               Settings &settings)
    : suspensionStateMachineData(suspensionStateMachineData),
      suspensionStateMachineCommunication(suspensionStateMachineCommunication),
      solenoidManager(solenoidManager),
      pressureSensorManager(pressureSensorManager),
      logHandler(logHandler),
      settings(settings) {}

SuspensionStateMachine::~SuspensionStateMachine() {
    currentState->Leave();
    suspensionStateMachineCommunication.Leave();
    delete currentState;
    currentState = nullptr;
}

void SuspensionStateMachine::Begin() {
    suspensionStateMachineCommunication.Init();
    suspensionStateMachineData.newRequestedState = EState::IDLE;
    ChangeState(suspensionStateMachineData.newRequestedState);
}

void SuspensionStateMachine::Loop() {
    if (currentState) {
        EState stateRequestedByState = currentState->Loop();
        if (currentState->GetEState() != suspensionStateMachineData.newRequestedState) {
            ChangeState(suspensionStateMachineData.newRequestedState);
            return;
        }
        if (currentState->GetEState() != stateRequestedByState) {
            ChangeState(stateRequestedByState);
        }
    }
}

void SuspensionStateMachine::Leave() {
    suspensionStateMachineCommunication.Leave();
}

void SuspensionStateMachine::ChangeState(EState newState) {
    if (currentState) {
        currentState->Leave();
        delete currentState;
    }
    LOG_DEBUG("Suspension changing state to:", static_cast<int>(newState));
    switch (newState) {
        case EState::IDLE:
            currentState = new IdleState(solenoidManager);
            break;
        case EState::FRONT_UP:
            currentState = new FrontUpState(solenoidManager.GetSolenoid(ESolenoid::FRONT_UP), logHandler);
            break;
        case EState::FRONT_DOWN:
            currentState = new FrontDownState(solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN), logHandler);
            break;
        case EState::BACK_UP:
            currentState = new BackUpState(solenoidManager.GetSolenoid(ESolenoid::BACK_UP), logHandler);
            break;
        case EState::BACK_DOWN:
            currentState = new BackDownState(solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN), logHandler);
            break;
        case EState::PARK:
            currentState = new ParkState(solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN),
                                         solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN), logHandler, settings);
            break;
        case EState::RIDE:
            currentState = new RideState(solenoidManager, logHandler, settings,
                                         pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT),
                                         pressureSensorManager.GetPressureSensor(EPressureSensor::BACK));
            break;
    }
    suspensionStateMachineData.newRequestedState = newState;
    currentState->Enter();
}
