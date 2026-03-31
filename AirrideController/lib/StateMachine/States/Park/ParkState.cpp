#include "ParkState.h"
#include "Arduino.h"

ParkState::ParkState(Solenoid &frontSolenoid, Solenoid &backSolenoid, LogHandler &logHandler, Settings &settings)
    : frontSolenoid(frontSolenoid), backSolenoid(backSolenoid), logHandler(logHandler), settings(settings) {
}

void ParkState::Enter() {
    logHandler.StartFrontLog();
    logHandler.StartBackLog();
    frontSolenoid.TurnOn();
    backSolenoid.TurnOn();
    timeInterval = settings.parkDuration * 1000;
    timePrevious = millis();
}

void ParkState::Leave() {
    frontSolenoid.TurnOff();
    backSolenoid.TurnOff();
    logHandler.EndFrontLog();
    logHandler.EndBackLog();
}

EState ParkState::Loop() {
    timeNow = millis();
    if (timeNow - timePrevious > timeInterval) {
        return EState::IDLE;
    }
    return EState::PARK;
}
