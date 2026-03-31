#include "PressureSensor.h"
#include <Arduino.h>

void PressureSensor::Begin() {
    buffer.clear();
    buffer.reserve(filterSize);
    bufferTotal = 0;
    for (int i = 0; i < filterSize; i++) {
        buffer.push_back(ReadPressure());
        bufferTotal += buffer[i];
    }
}

void PressureSensor::UpdateBuffer() {
    bufferTotal = bufferTotal - buffer[bufferIndex];
    buffer[bufferIndex] = ReadPressure();
    bufferTotal = bufferTotal + buffer[bufferIndex];
    bufferIndex = (bufferIndex + 1) % filterSize;
    bufferAverage = bufferTotal / filterSize;
}

PressureSensor::~PressureSensor() {
    buffer.clear();
}

float PressureSensor::GetRawPressure() {
    return ReadPressure();
}

float PressureSensor::ReadPressure() {
    return FloatMap(analogRead(pin), analogMin, analogMax, 0, barMax);
}

float PressureSensor::FloatMap(float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
