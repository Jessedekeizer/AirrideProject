#include "RideState.h"
#include "Arduino.h"

RideState::RideState(SolenoidManager &solenoidManager, LogHandler &logHandler, Settings &settings,
                     PressureSensor &frontPressureSensor, PressureSensor &backPressureSensor)
    : solenoidManager(solenoidManager), logHandler(logHandler), settings(settings),
      frontPressureSensor(frontPressureSensor), backPressureSensor(backPressureSensor) {
}

void RideState::Enter() {
    SetupRide();
}

void RideState::SetupRide() {
    rideFrontDone = false;
    rideBackDone = false;

    double currentFront = frontPressureSensor.GetAveragePressure();
    double currentBack = backPressureSensor.GetAveragePressure();

    if (currentFront > settings.rideFront) {
        frontTimePrevious = millis();
        frontTimeInterval = CalculateTime(currentFront, settings.rideFront, settings.frontDownX);

        frontSolenoid = &solenoidManager.GetSolenoid(ESolenoid::FRONT_DOWN);
        frontSolenoid->TurnOn();
    }
    else if (currentFront < settings.rideFront) {
        frontTimePrevious = millis();
        frontTimeInterval = CalculateTime(currentFront, settings.rideFront, settings.frontUpX);
        frontSolenoid = &solenoidManager.GetSolenoid(ESolenoid::FRONT_UP);
        frontSolenoid->TurnOn();
    }
    else {
        rideFrontDone = true;
    }

    if (currentBack > settings.rideBack) {
        backTimePrevious = millis();
        backTimeInterval = CalculateTime(currentBack, settings.rideBack, settings.backDownX);

        backSolenoid = &solenoidManager.GetSolenoid(ESolenoid::BACK_DOWN);
        backSolenoid->TurnOn();
    }
    else if (currentBack < settings.rideBack) {
        backTimePrevious = millis();
        backTimeInterval = CalculateTime(currentBack, settings.rideBack, settings.backUpX);
        backSolenoid = &solenoidManager.GetSolenoid(ESolenoid::BACK_UP);
        backSolenoid->TurnOn();
    }
    else {
        rideBackDone = true;
    }

    if (!rideFrontDone && !rideBackDone) {
        logHandler.StartFrontLog(true);
        logHandler.StartBackLog(true);
    }
    else {
        logHandler.StartFrontLog();
        logHandler.StartBackLog();
    }
}

long RideState::CalculateTime(double current, double desired, double x) {
    return ((x * desired) - (x * current)) * 1000;
}

void RideState::Leave() {
    frontSolenoid = nullptr;
    backSolenoid = nullptr;
}

EState RideState::Loop() {
    long timeNow = millis();
    if (!rideFrontDone && (timeNow - frontTimePrevious > frontTimeInterval)) {
        frontSolenoid->TurnOff();
        logHandler.EndFrontLog();
        rideFrontDone = true;
    }

    if (!rideBackDone && (timeNow - backTimePrevious > backTimeInterval)) {
        backSolenoid->TurnOff();
        logHandler.EndBackLog();
        rideBackDone = true;
    }

    if (rideFrontDone && rideBackDone) {
        return EState::IDLE;
    }
    return EState::RIDE;
}


