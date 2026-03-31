#include "BackUpState.h"

BackUpState::BackUpState(Solenoid &backUpSolenoid, LogHandler &logHandler)
    : backUpSolenoid(backUpSolenoid), logHandler(logHandler) {
}

void BackUpState::Enter() {
    logHandler.StartBackLog();
    backUpSolenoid.TurnOn();
}

void BackUpState::Leave() {
    backUpSolenoid.TurnOff();
    logHandler.EndBackLog();
}
