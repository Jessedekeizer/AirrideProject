#include "FrontUpState.h"

FrontUpState::FrontUpState(Solenoid &frontUpSolenoid, LogHandler &logHandler)
    : frontUpSolenoid(frontUpSolenoid), logHandler(logHandler) {
}

void FrontUpState::Enter() {
    logHandler.StartFrontLog();
    frontUpSolenoid.TurnOn();
}

void FrontUpState::Leave() {
    frontUpSolenoid.TurnOff();
    logHandler.EndFrontLog();
}
