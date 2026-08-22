#ifndef FRONTUPSTATE_H
#define FRONTUPSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "ISolenoid.h"


class FrontUpState : public IState {
public:
    FrontUpState(ISolenoid &frontUpSolenoid, LogHandler &logHandler);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; }

private:
    const EState state = EState::FRONT_UP;
    ISolenoid &frontUpSolenoid;
    LogHandler &logHandler;
};


#endif //FRONTUPSTATE_H
