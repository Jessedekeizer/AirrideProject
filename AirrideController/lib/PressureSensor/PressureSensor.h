#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H
#include <vector>

#include "EPressureSensor.h"

struct PressureSensorFilterConfig {
    int filterSize;
    float analogMin;
    float analogMax;
    float barMax;
};

class PressureSensor {
public:
    PressureSensor(EPressureSensor pressureSensor, int pin, PressureSensorFilterConfig config)
        : pin(pin), pressureSensor(pressureSensor), filterSize(config.filterSize), analogMin(config.analogMin),
          analogMax(config.analogMax), barMax(config.barMax) {
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
