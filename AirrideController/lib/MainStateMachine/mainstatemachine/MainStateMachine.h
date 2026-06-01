#ifndef MAINCONTROLLERSTATEMACHINE_H
#define MAINCONTROLLERSTATEMACHINE_H
#include "IMainState.h"
#include "MainStateMachineData.h"
#include "SuspensionStateMachine.h"
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "PressureSensor.h"
#include "SuspensionStateCommunication.h"
#include "SolenoidManager.h"
#include "OTACommunication.h"

class MainStateMachine {
public:
    MainStateMachine(MainStateMachineData &data,
                               SuspensionStateMachine &suspensionStateMachine,
                               LogHandler &logHandler,
                               PressureSensorManager &pressureSensorManager,
                               SuspensionStateCommunication &suspensionStateCommunication,
                               SolenoidManager &solenoidManager,
                               OTACommunication &otaCommunication);

    ~MainStateMachine();

    void Begin();
    void Loop();

private:
    void ChangeState(EMainState newState);

    MainStateMachineData &data;
    IMainState *currentState = nullptr;

    SuspensionStateMachine &suspensionStateMachine;
    LogHandler &logHandler;
    PressureSensorManager &pressureSensorManager;
    SuspensionStateCommunication &suspensionStateCommunication;
    SolenoidManager &solenoidManager;
    OTACommunication &otaCommunication;
};

#endif //MAINCONTROLLERSTATEMACHINE_H
