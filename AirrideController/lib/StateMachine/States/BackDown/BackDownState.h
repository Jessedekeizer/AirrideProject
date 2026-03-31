#ifndef BACKDOWNSTATE_H
#define BACKDOWNSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "Solenoid.h"


class BackDownState : public IState {
public:
    BackDownState(Solenoid &backUpSolenoid, LogHandler &logHandler);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; };

private:
    const EState state = EState::BACK_DOWN;
    Solenoid &backDownSolenoid;
    LogHandler &logHandler;
};


#endif //BACKDOWNSTATE_H
