#ifndef PARKSTATE_H
#define PARKSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "ISolenoid.h"


class ParkState : public IState {
public:
    ParkState(ISolenoid &frontSolenoid, ISolenoid &backSolenoid, LogHandler &logHandler, Settings &settings);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override;

private:
    const EState state = EState::PARK;
    ISolenoid &frontSolenoid;
    ISolenoid &backSolenoid;
    LogHandler &logHandler;
    Settings &settings;
    long timePrevious = 0;
    long timeNow = 0;

    long timeInterval = 4000;
};


#endif //PARKSTATE_H
