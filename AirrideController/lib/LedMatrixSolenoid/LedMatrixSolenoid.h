#ifndef LEDMATRIXSOLENOID_H
#define LEDMATRIXSOLENOID_H
#include "ISolenoid.h"
#include <Arduino_LED_Matrix.h>

// LED matrix layout (12 cols x 8 rows), one 3-col section per solenoid:
//  FRONT_UP  | FRONT_DOWN | BACK_UP  | BACK_DOWN
//  cols 0-2  |  cols 3-5  | cols 6-8 | cols 9-11

class LedMatrixSolenoid : public ISolenoid {
public:
    LedMatrixSolenoid(ESolenoid solenoid, int colStart, bool active = true);

    void Begin() override;
    void TurnOn() override;
    void TurnOff() override;
    ESolenoid GetESolenoid() override { return solenoid; }
    void Activate(bool newActive) override;

private:
    void Render();

    ESolenoid solenoid;
    int colStart;
    bool active;
    bool isOn;

    static ArduinoLEDMatrix matrix;
    static uint8_t frame[8][12];
    static bool initialized;
};

#endif //LEDMATRIXSOLENOID_H
