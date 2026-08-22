#include "FrontUpState.h"

FrontUpState::FrontUpState(ISolenoid &frontUpISolenoid, LogHandler &logHandler)
    : frontUpSolenoid(frontUpISolenoid), logHandler(logHandler) {
}

void FrontUpState::Enter() {
    logHandler.StartFrontLog();
    frontUpSolenoid.TurnOn();
}

void FrontUpState::Leave() {
    frontUpSolenoid.TurnOff();
    logHandler.EndFrontLog();
}
