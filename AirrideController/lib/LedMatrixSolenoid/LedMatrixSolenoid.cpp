#include "LedMatrixSolenoid.h"
#include <string.h>

ArduinoLEDMatrix LedMatrixSolenoid::matrix;
uint8_t LedMatrixSolenoid::frame[8][12] = {};
bool LedMatrixSolenoid::initialized = false;

LedMatrixSolenoid::LedMatrixSolenoid(ESolenoid solenoid, int colStart, bool active)
    : solenoid(solenoid), colStart(colStart), active(active), isOn(false) {
}

void LedMatrixSolenoid::Begin() {
    if (!initialized) {
        matrix.begin();
        memset(frame, 0, sizeof(frame));
        initialized = true;
    }
}

void LedMatrixSolenoid::TurnOn() {
    if (active) {
        isOn = true;
        Render();
    }
}

void LedMatrixSolenoid::TurnOff() {
    isOn = false;
    Render();
}

void LedMatrixSolenoid::Activate(bool newActive) {
    active = newActive;
    if (!active && isOn) {
        isOn = false;
        Render();
    }
}

void LedMatrixSolenoid::Render() {
    for (int row = 0; row < 8; row++) {
        for (int col = colStart; col < colStart + 3; col++) {
            frame[row][col] = isOn ? 1 : 0;
        }
    }
    matrix.renderBitmap(frame, 8, 12);
}
