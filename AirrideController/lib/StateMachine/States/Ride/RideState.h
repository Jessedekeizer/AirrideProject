#ifndef RIDESTATE_H
#define RIDESTATE_H
#include "IState.h"
#include "LogHandler.h"
#include "PressureSensor.h"
#include "Settings.h"
#include "Solenoid.h"
#include "SolenoidManager.h"

class RideState : public IState {
public:
    RideState(SolenoidManager &solenoidManager, LogHandler &logHandler, Settings &settings,
              PressureSensor &frontPressureSensor, PressureSensor &backPressureSensor);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override;

private:
    const EState state = EState::RIDE;
    Solenoid *frontSolenoid;
    Solenoid *backSolenoid;
    PressureSensor &frontPressureSensor;
    PressureSensor &backPressureSensor;
    LogHandler &logHandler;
    Settings &settings;
    SolenoidManager &solenoidManager;

    bool rideFrontDone = false;
    bool rideBackDone = false;

    long frontTimeInterval;
    long frontTimePrevious;
    long backTimeInterval;
    long backTimePrevious;

    long CalculateTime(double current, double desired, double x);

    void SetupRide();
};

#endif //RIDESTATE_H
