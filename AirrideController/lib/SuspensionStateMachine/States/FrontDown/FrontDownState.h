#ifndef FRONTDOWNSTATE_H
#define FRONTDOWNSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "ISolenoid.h"

class FrontDownState : public IState {
public:
    FrontDownState(ISolenoid &frontDownSolenoid, LogHandler &logHandler);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; }

private:
    const EState state = EState::FRONT_DOWN;
    ISolenoid &frontDownSolenoid;
    LogHandler &logHandler;
};


#endif //FRONTDOWNSTATE_H
