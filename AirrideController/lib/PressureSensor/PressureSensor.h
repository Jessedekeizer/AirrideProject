#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H
#include <vector>

#include "EPressureSensor.h"

class PressureSensor {
public:
    PressureSensor(EPressureSensor pressureSensor, int pin, int filterSize, float analogMin, float analogMax,
                   float barMax)
        : pressureSensor(pressureSensor), pin(pin), filterSize(filterSize), analogMin(analogMin), analogMax(analogMax),
          barMax(barMax) {
    }

    ~PressureSensor();

    EPressureSensor GetPressureSensor() { return pressureSensor; }

    float GetAveragePressure() { return bufferAverage; }

    float GetRawPressure();

    void UpdateBuffer();

    void Begin();

private:
    const int pin;
    const EPressureSensor pressureSensor;
    const int filterSize;

    std::vector<float> buffer;
    int bufferIndex = 0;
    float bufferTotal = 0;
    float bufferAverage = 0;
    float analogMin;
    float analogMax;
    float barMax;

    float ReadPressure();

    float FloatMap(float x, float in_min, float in_max, float out_min, float out_max);
};


#endif //PRESSURESENSOR_H
