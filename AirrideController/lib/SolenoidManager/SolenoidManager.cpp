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

void SolenoidManager::AddSolenoid(Solenoid &solenoid) {
    solenoids.push_back(&solenoid);
}

Solenoid &SolenoidManager::GetSolenoid(ESolenoid requestedSolenoid) {
    for (auto solenoid: solenoids) {
        if (solenoid->GetESolenoid() == requestedSolenoid) {
            return *solenoid;
        }
    }
}

void SolenoidManager::TurnOffAllSolenoids() {
    for (auto solenoid: solenoids) {
        solenoid->TurnOff();
    }
}


