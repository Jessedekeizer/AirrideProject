#include "FrontDownState.h"

FrontDownState::FrontDownState(ISolenoid &frontDownISolenoid, LogHandler &logHandler)
    : frontDownSolenoid(frontDownISolenoid), logHandler(logHandler) {
}

void FrontDownState::Enter() {
    logHandler.StartFrontLog();
    frontDownSolenoid.TurnOn();
}

void FrontDownState::Leave() {
    frontDownSolenoid.TurnOff();
    logHandler.EndFrontLog();
}
