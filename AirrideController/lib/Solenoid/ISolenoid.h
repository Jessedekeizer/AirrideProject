#ifndef ISOLENOID_H
#define ISOLENOID_H
#include "ESolenoid.h"

class ISolenoid {
public:
    virtual ~ISolenoid() = default;
    virtual void Begin() = 0;
    virtual void TurnOn() = 0;
    virtual void TurnOff() = 0;
    virtual ESolenoid GetESolenoid() = 0;
    virtual void Activate(bool active) = 0;
};

#endif //ISOLENOID_H
