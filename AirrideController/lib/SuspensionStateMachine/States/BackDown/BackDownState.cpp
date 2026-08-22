#include "BackDownState.h"

BackDownState::BackDownState(ISolenoid &backDownISolenoid, LogHandler &logHandler)
    : backDownSolenoid(backDownISolenoid), logHandler(logHandler) {
}

void BackDownState::Enter() {
    logHandler.StartBackLog();
    backDownSolenoid.TurnOn();
}

void BackDownState::Leave() {
    backDownSolenoid.TurnOff();
    logHandler.EndBackLog();
}
