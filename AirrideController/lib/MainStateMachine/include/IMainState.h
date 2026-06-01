#ifndef IMAINCONTROLLERSTATE_H
#define IMAINCONTROLLERSTATE_H
#include "EMainState.h"

class IMainState {
public:
    virtual ~IMainState() = default;

    virtual EMainState GetState() = 0;
    virtual void Enter() = 0;
    virtual EMainState Loop() = 0;
    virtual void Leave() = 0;
};

#endif //IMAINCONTROLLERSTATE_H
