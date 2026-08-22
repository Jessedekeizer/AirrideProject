#ifndef SOLENOIDMANAGER_H
#define SOLENOIDMANAGER_H
#include <vector>

#include "ISolenoid.h"

class SolenoidManager {
public:
    SolenoidManager();

    ~SolenoidManager();

    void Begin();

    void AddSolenoid(ISolenoid &solenoid);

    ISolenoid &GetSolenoid(ESolenoid requestedSolenoid);

    void TurnOffAllSolenoids();

private:
    std::vector<ISolenoid *> solenoids;
};

#endif //SOLENOIDMANAGER_H
