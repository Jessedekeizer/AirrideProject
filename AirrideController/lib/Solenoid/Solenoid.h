#ifndef SOLENOID_H
#define SOLENOID_H
#include "ESolenoid.h"

class Solenoid {
public:
    Solenoid(ESolenoid solenoid, int pin, bool active = true) : solenoid(solenoid), pin(pin), active(active) {
    }

    ~Solenoid() {
    };

    void Begin();

    void TurnOn();

    void TurnOff();

    ESolenoid GetESolenoid() { return solenoid; }

    void Activate(bool active);

private:
    ESolenoid solenoid;
    const int pin;
    bool active;
};

#endif //SOLENOID_H
