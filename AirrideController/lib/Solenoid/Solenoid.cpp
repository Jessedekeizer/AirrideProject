#include "Solenoid.h"

#include <Arduino.h>

void Solenoid::Begin() {
    pinMode(pin, OUTPUT);
}

void Solenoid::TurnOn() {
    if (active) {
        digitalWrite(pin, HIGH);
    }
}

void Solenoid::TurnOff() {
    digitalWrite(pin, LOW);
}

void Solenoid::Activate(bool newActiveState) {
    active = newActiveState;
}
