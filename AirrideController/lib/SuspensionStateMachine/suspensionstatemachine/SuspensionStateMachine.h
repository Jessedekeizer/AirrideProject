#ifndef SUSPENSIONSTATEMACHINE_H
#define SUSPENSIONSTATEMACHINE_H
#include "SolenoidManager.h"
#include "IState.h"
#include "LogHandler.h"
#include "SuspensionStateMachineCommunication.h"
#include "SuspensionStateMachineData.h"
#include "PressureSensorManager.h"

class SuspensionStateMachine {
public:
    SuspensionStateMachine(SuspensionStateMachineData &suspensionStateMachineData,
                           SuspensionStateMachineCommunication &suspensionStateMachineCommunication,
                           SolenoidManager &solenoidManager,
                           PressureSensorManager &pressureSensorManager,
                           LogHandler &logHandler,
                           Settings &settings);

    ~SuspensionStateMachine();

    void Begin();
    void Loop();
    void Leave();

private:
    void ChangeState(EState newState);

    IState *currentState = nullptr;
    SolenoidManager &solenoidManager;
    LogHandler &logHandler;
    Settings &settings;
    PressureSensorManager &pressureSensorManager;
    SuspensionStateMachineCommunication &suspensionStateMachineCommunication;
    SuspensionStateMachineData &suspensionStateMachineData;
};

#endif //SUSPENSIONSTATEMACHINE_H
