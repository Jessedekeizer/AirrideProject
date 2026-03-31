#ifndef SCREENMANAGER_H
#define SCREENMANAGER_H
#include "IScreen.h"
#include <memory>
#include <vector>
#include "DisplayService.h"

class ScreenManager {
public:
    ScreenManager(DisplayService &displayService);
    ~ScreenManager();
    void AddScreen(IScreen *screen);
    bool RequestScreen(EScreen requestedScreen);
    IScreen *GetActiveScreen() const { return activeScreen; }

private:
    DisplayService &displayService;
    std::vector<IScreen *> screens;
    IScreen *activeScreen;

    IScreen *FindScreen(EScreen newScreen);
    void TransitionToScreen(IScreen *newScreen);
};

#endif
