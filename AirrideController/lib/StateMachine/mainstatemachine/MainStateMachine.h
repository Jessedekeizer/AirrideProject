#ifndef MAINSTATEMACHINE_H
#define MAINSTATEMACHINE_H
#include "SolenoidManager.h"
#include "IState.h"
#include "LogHandler.h"
#include "MainStateMachineCommunication.h"
#include "MainStateMachineData.h"
#include "PressureSensorManager.h"

class MainStateMachine {
public:
    MainStateMachine(MainStateMachineData &mainStateMachineData,
                     MainStateMachineCommunication &mainStateMachineCommunication,
                     SolenoidManager &solenoidManager,
                     PressureSensorManager &pressureSensorManager,
                     LogHandler &logHandler,
                     Settings &settings);

    ~MainStateMachine();

    void Begin();

    void Loop();

private:
    void ChangeState(EState newState);

    IState *currentState;
    SolenoidManager &solenoidManager;
    LogHandler &logHandler;
    Settings &settings;
    PressureSensorManager &pressureSensorManager;
    MainStateMachineCommunication &mainStateMachineCommunication;
    MainStateMachineData &mainStateMachineData;
};

#endif //MAINSTATEMACHINE_H
