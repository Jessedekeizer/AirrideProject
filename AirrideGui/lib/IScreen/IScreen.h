#ifndef ISCREEN_H
#define ISCREEN_H

#include <Arduino.h>
#include <Button.h>
#include <vector>

#include "EScreens.h"

class IScreen {
public:
    virtual ~IScreen() = default;

    virtual void HandleTouch(int touchX, int touchY) = 0;

    virtual void OnLoop() = 0;

    virtual void ReleaseButtons() = 0;

    virtual void OnSetup() = 0;

    virtual EScreen GetName() = 0;

    virtual const char *GetPath() = 0;
};

#endif
