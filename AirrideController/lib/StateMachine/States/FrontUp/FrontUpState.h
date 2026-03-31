#ifndef FRONTUPSTATE_H
#define FRONTUPSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "Solenoid.h"


class FrontUpState : public IState {
public:
    FrontUpState(Solenoid &frontUpSolenoid, LogHandler &logHandler);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; }

private:
    const EState state = EState::FRONT_UP;
    Solenoid &frontUpSolenoid;
    LogHandler &logHandler;
};


#endif //FRONTUPSTATE_H
