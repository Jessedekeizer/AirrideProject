#include "SuspensionState.h"

SuspensionState::SuspensionState(SuspensionStateMachine &suspensionStateMachine,
                                 LogHandler &logHandler,
                                 PressureSensorManager &pressureSensorManager,
                                 SuspensionStateCommunication &communication)
    : suspensionStateMachine(suspensionStateMachine),
      logHandler(logHandler),
      pressureSensorManager(pressureSensorManager),
      communication(communication) {}

EMainState SuspensionState::GetState() {
    return EMainState::SUSPENSION;
}

void SuspensionState::Enter() {
    lastSensorWake = millis();
    suspensionStateMachine.Begin();
}

EMainState SuspensionState::Loop() {
    suspensionStateMachine.Loop();
    logHandler.SendLog();

    if (millis() - lastSensorWake >= sensorInterval) {
        pressureSensorManager.Update();
        communication.SendPressure(
            pressureSensorManager.GetPressureSensor(EPressureSensor::FRONT).GetRawPressure(),
            pressureSensorManager.GetPressureSensor(EPressureSensor::BACK).GetRawPressure()
        );
        lastSensorWake = millis();
    }

    return EMainState::SUSPENSION;
}

void SuspensionState::Leave() {
    suspensionStateMachine.Leave();
}
