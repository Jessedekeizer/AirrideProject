#include "BackDownState.h"

BackDownState::BackDownState(Solenoid &backDownSolenoid, LogHandler &logHandler)
    : backDownSolenoid(backDownSolenoid), logHandler(logHandler) {
}

void BackDownState::Enter() {
    logHandler.StartBackLog();
    backDownSolenoid.TurnOn();
}

void BackDownState::Leave() {
    backDownSolenoid.TurnOff();
    logHandler.EndBackLog();
}
