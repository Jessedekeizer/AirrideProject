#ifndef IDLESTATE_H
#define IDLESTATE_H
#include "../../include/IState.h"
#include "SolenoidManager.h"


class IdleState : public IState {
public:
    IdleState(SolenoidManager &solenoidManager);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override {
    }

    EState Loop() override { return state; }

private:
    const EState state = EState::IDLE;
    SolenoidManager &solenoidManager;
};


#endif //IDLESTATE_H
