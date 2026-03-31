#include "PressureSensorManager.h"
#include <Arduino.h>

PressureSensorManager::PressureSensorManager(Solenoid &frontSolenoid, Solenoid &backSolenoid, Settings &settings)
    : frontSolenoid(frontSolenoid), backSolenoid(backSolenoid), settings(settings) {
}

PressureSensorManager::~PressureSensorManager() {
    pressureSensors.clear();
}

void PressureSensorManager::Begin() {
    for (auto pressureSensor: pressureSensors) {
        pressureSensor->Begin();
    }
}

void PressureSensorManager::AddPressureSensor(PressureSensor &pressureSensor) {
    pressureSensors.push_back(&pressureSensor);
}

void PressureSensorManager::Update() {
    for (auto pressureSensor: pressureSensors) {
        pressureSensor->UpdateBuffer();
        if (pressureSensor->GetPressureSensor() == EPressureSensor::FRONT) {
            CheckIfPressureIsWithinTolerance(pressureSensor, frontSolenoid);
        } else if (pressureSensor->GetPressureSensor() == EPressureSensor::BACK) {
            CheckIfPressureIsWithinTolerance(pressureSensor, backSolenoid);
        }
    }
}

PressureSensor &PressureSensorManager::GetPressureSensor(EPressureSensor requestedPressureSensor) {
    for (auto pressureSensor: pressureSensors) {
        if (pressureSensor->GetPressureSensor() == requestedPressureSensor) {
            return *pressureSensor;
        }
    }
}

void PressureSensorManager::CheckIfPressureIsWithinTolerance(PressureSensor *pressureSensor, Solenoid &solenoid) {
    //Turn off solenoid when max pressure is reached.
    if (pressureSensor->GetAveragePressure() >= settings.frontMax) {
        solenoid.Activate(false);
    } else {
        solenoid.Activate(true);
    }
}
