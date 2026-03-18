#include "MainStateMachine.h"
#include "BackDownState.h"
#include "BackUpState.h"
#include "FrontDownState.h"
#include "FrontUpState.h"
#include "IdleState.h"
#include "ParkState.h"
#include "RideState.h"

#include "Logger.h"

MainStateMachine::MainStateMachine(MainStateMachineData &mainStateMachineData,
                                   MainStateMachineCommunication &mainStateMachineCommunication,
                                   SolenoidManager &solenoidManager,
                                   PressureSensorManager &pressureSensorManager,
                                   LogHandler &logHandler,
                                   Settings &settings) : mainStateMachineData(mainStateMachineData),
                                                         mainStateMachineCommunication(mainStateMachineCommunication),
                                                         solenoidManager(solenoidManager),
                                                         pressureSensorManager(pressureSensorManager),
                                                         logHandler(logHandler),
                                                         settings(settings) {
}

MainStateMachine::~MainStateMachine() {
    currentState->Leave();
    mainStateMachineCommunication.Leave();
    delete currentState;
    currentState = nullptr;
}

void MainStateMachine::Begin() {
    mainStateMachineCommunication.Init();
    mainStateMachineData.newRequestedState = EState::IDLE;
    ChangeState(mainStateMachineData.newRequestedState);
}

void MainStateMachine::Loop() {
    if (currentState) {
        EState stateRequestedByState = currentState->Loop();
        if (currentState->GetEState() != mainStateMachineData.newRequestedState) {
            ChangeState(mainStateMachineData.newRequestedState);
            return;
        }
        if (currentState->GetEState() != stateRequestedByState) {
            ChangeState(stateRequestedByState);
        }
    }
}

void MainStateMachine::ChangeState(EState newState) {
    if (currentState) {
        currentState->Leave();
        delete currentState;
    }
    LOG_DEBUG("Changing state to:", static_cast<int>(newState));
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
    mainStateMachineData.newRequestedState = newState;
    currentState->Enter();
}
