#include "IdleState.h"

IdleState::IdleState(SolenoidManager &solenoidManager) : solenoidManager(solenoidManager) {
}

void IdleState::Enter() {
    solenoidManager.TurnOffAllSolenoids();
}
