#ifndef CONTROLLINGSTATE_H
#define CONTROLLINGSTATE_H
#include "IMainState.h"
#include "SuspensionStateMachine.h"
#include "LogHandler.h"
#include "PressureSensorManager.h"

#include "SuspensionStateCommunication.h"

class SuspensionState : public IMainState {
public:
    SuspensionState(SuspensionStateMachine &suspensionStateMachine,
                     LogHandler &logHandler,
                     PressureSensorManager &pressureSensorManager,
                     SuspensionStateCommunication &communication);

    EMainState GetState() override;
    void Enter() override;
    EMainState Loop() override;
    void Leave() override;

private:
    SuspensionStateMachine &suspensionStateMachine;
    LogHandler &logHandler;
    PressureSensorManager &pressureSensorManager;
    SuspensionStateCommunication &communication;

    unsigned long lastSensorWake = 0;
    static constexpr TickType_t sensorInterval = 200;
};

#endif //CONTROLLINGSTATE_H
