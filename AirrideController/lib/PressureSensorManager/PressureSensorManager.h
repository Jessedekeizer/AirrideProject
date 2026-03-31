#ifndef PRESSURESENSORMANAGER_H
#define PRESSURESENSORMANAGER_H
#include <vector>
#include "PressureSensor.h"
#include "Settings.h"
#include "Solenoid.h"

class PressureSensorManager {
public:
    PressureSensorManager(Solenoid &frontSolenoid, Solenoid &backSolenoid, Settings &settings);

    ~PressureSensorManager();

    PressureSensor &GetPressureSensor(EPressureSensor requestedPressureSensor);

    void Update();

    void Begin();
    void AddPressureSensor(PressureSensor &pressureSensor);

private:
    void CheckIfPressureIsWithinTolerance(PressureSensor *pressureSensor, Solenoid &solenoid);

    std::vector<PressureSensor *> pressureSensors;
    Solenoid &frontSolenoid;
    Solenoid &backSolenoid;
    Settings &settings;
};

#endif //PRESSURESENSORMANAGER_H
