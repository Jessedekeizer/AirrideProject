#ifndef BACKUPSTATE_H
#define BACKUPSTATE_H
#include "../../include/IState.h"
#include "LogHandler.h"
#include "ISolenoid.h"


class BackUpState : public IState {
public:
    BackUpState(ISolenoid &backUpSolenoid, LogHandler &logHandler);

    EState GetEState() override { return state; }

    void Enter() override;

    void Leave() override;

    EState Loop() override { return state; };

private:
    const EState state = EState::BACK_UP;
    ISolenoid &backUpSolenoid;
    LogHandler &logHandler;
};


#endif //BACKUPSTATE_H
