#ifndef SOLENOIDMANAGER_H
#define SOLENOIDMANAGER_H
#include <vector>

#include "Solenoid.h"

class SolenoidManager {
public:
    SolenoidManager();

    ~SolenoidManager();

    void Begin();

    void AddSolenoid(Solenoid &solenoid);

    Solenoid &GetSolenoid(ESolenoid requestedSolenoid);

    void TurnOffAllSolenoids();

private:
    std::vector<Solenoid *> solenoids;
};

#endif //SOLENOIDMANAGER_H
