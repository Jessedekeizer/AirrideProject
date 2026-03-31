#ifndef STATE_H
#define STATE_H
#include "EState.h"

class IState {
public:
    virtual ~IState() = default;

    IState() {
    }

    virtual EState GetEState() = 0;

    virtual void Enter() = 0;

    virtual EState Loop() = 0;

    virtual void Leave() = 0;
};

#endif //STATE_H
