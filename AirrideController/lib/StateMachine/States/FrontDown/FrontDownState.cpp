#include "FrontDownState.h"

FrontDownState::FrontDownState(Solenoid &frontDownSolenoid, LogHandler &logHandler)
    : frontDownSolenoid(frontDownSolenoid), logHandler(logHandler) {
}

void FrontDownState::Enter() {
    logHandler.StartFrontLog();
    frontDownSolenoid.TurnOn();
}

void FrontDownState::Leave() {
    frontDownSolenoid.TurnOff();
    logHandler.EndFrontLog();
}
