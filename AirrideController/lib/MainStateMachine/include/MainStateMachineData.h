#ifndef MAINCONTROLLERDATA_H
#define MAINCONTROLLERDATA_H
#include "EMainState.h"

struct MainStateMachineData {
    EMainState requestedState = EMainState::SUSPENSION;
};

#endif //MAINCONTROLLERDATA_H
