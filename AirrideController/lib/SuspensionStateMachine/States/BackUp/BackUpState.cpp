#include "BackUpState.h"

BackUpState::BackUpState(ISolenoid &backUpISolenoid, LogHandler &logHandler)
    : backUpSolenoid(backUpISolenoid), logHandler(logHandler) {
}

void BackUpState::Enter() {
    logHandler.StartBackLog();
    backUpSolenoid.TurnOn();
}

void BackUpState::Leave() {
    backUpSolenoid.TurnOff();
    logHandler.EndBackLog();
}
