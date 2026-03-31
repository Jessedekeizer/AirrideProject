#include "ScreenManager.h"

#include "Logger.h"

ScreenManager::ScreenManager(DisplayService &displayService)
    : displayService(displayService) {
}

ScreenManager::~ScreenManager() {
    activeScreen = nullptr;
    screens.clear();
}

void ScreenManager::AddScreen(IScreen *screen) {
    screens.push_back(screen);
}

bool ScreenManager::RequestScreen(EScreen requestedScreen) {
    LOG_DEBUG("ScreenManager::ChangeScreen - Changing to:", static_cast<int>(requestedScreen));

    IScreen *newScreen = FindScreen(requestedScreen);
    if (!newScreen || newScreen == activeScreen)
        return false;
    LOG_DEBUG("ScreenManager::ChangeScreen - Transitioning to screen:", static_cast<int>(newScreen->GetName()));
    TransitionToScreen(newScreen);
    return true;
}

IScreen *ScreenManager::FindScreen(EScreen newScreen) {
    for (IScreen *screen: screens) {
        if (screen->GetName() == newScreen) {
            return screen;
        }
    }
    return nullptr;
}

void ScreenManager::TransitionToScreen(IScreen *newScreen) {
    if (activeScreen) {
        activeScreen->ReleaseButtons();
    }
    activeScreen = newScreen;
    displayService.DrawImage(activeScreen->GetPath());
    LOG_DEBUG("Image drawn");
    activeScreen->OnSetup();
}
