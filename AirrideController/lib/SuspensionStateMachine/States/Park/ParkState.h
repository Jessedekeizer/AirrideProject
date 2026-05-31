#ifndef PARKSTATE_H
#define PARKSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "Solenoid.h"


class ParkState : public IState {
public:
    ParkState(Solenoid &frontSolenoid, Solenoid &backSolenoid, LogHandler &logHandler, Settings &settings);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override;

private:
    const EState state = EState::PARK;
    Solenoid &frontSolenoid;
    Solenoid &backSolenoid;
    LogHandler &logHandler;
    Settings &settings;
    long timePrevious = 0;
    long timeNow = 0;

    long timeInterval = 4000;
};


#endif //PARKSTATE_H
