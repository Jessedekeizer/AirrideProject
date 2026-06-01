#ifndef PRESSURESENSORMANAGER_H
#define PRESSURESENSORMANAGER_H
#include <vector>
#include "PressureSensor.h"
#include "Settings.h"
#include "ISolenoid.h"

class PressureSensorManager {
public:
    PressureSensorManager(ISolenoid &frontSolenoid, ISolenoid &backSolenoid, Settings &settings);

    ~PressureSensorManager();

    PressureSensor &GetPressureSensor(EPressureSensor requestedPressureSensor);

    void Update();

    void Begin();
    void AddPressureSensor(PressureSensor &pressureSensor);

private:
    void CheckIfPressureIsWithinTolerance(PressureSensor *pressureSensor, ISolenoid &solenoid);

    std::vector<PressureSensor *> pressureSensors;
    ISolenoid &frontSolenoid;
    ISolenoid &backSolenoid;
    Settings &settings;
};

#endif //PRESSURESENSORMANAGER_H
