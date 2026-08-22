#include "SolenoidManager.h"

SolenoidManager::SolenoidManager() {
}

SolenoidManager::~SolenoidManager() {
    solenoids.clear();
}

void SolenoidManager::Begin() {
    for (auto solenoid: solenoids) {
        solenoid->Begin();
    }
}

void SolenoidManager::AddSolenoid(ISolenoid &solenoid) {
    solenoids.push_back(&solenoid);
}

ISolenoid &SolenoidManager::GetSolenoid(ESolenoid requestedISolenoid) {
    for (auto solenoid: solenoids) {
        if (solenoid->GetESolenoid() == requestedISolenoid) {
            return *solenoid;
        }
    }
}

void SolenoidManager::TurnOffAllSolenoids() {
    for (auto solenoid: solenoids) {
        solenoid->TurnOff();
    }
}


