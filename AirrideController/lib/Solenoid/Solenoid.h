#ifndef SOLENOID_H
#define SOLENOID_H
#include "ISolenoid.h"

class Solenoid : public ISolenoid {
public:
    Solenoid(ESolenoid solenoid, int pin, bool active = true) : solenoid(solenoid), pin(pin), active(active) {
    }

    ~Solenoid() override {
    };

    void Begin() override;

    void TurnOn() override;

    void TurnOff() override;

    ESolenoid GetESolenoid() override { return solenoid; }

    void Activate(bool active) override;

private:
    ESolenoid solenoid;
    const int pin;
    bool active;
};

#endif //SOLENOID_H
