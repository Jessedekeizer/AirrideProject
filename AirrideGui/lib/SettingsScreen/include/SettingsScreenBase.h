#ifndef SETTINGSSCREENBASE_H
#define SETTINGSSCREENBASE_H
#include "BaseScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenCommunication.h"
#include "SettingsStorage.h"
#include "SettingsScreenGeometry.h"

class SettingsScreenBase : public BaseScreen {
public:
    SettingsScreenBase(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService, EScreen screen, const char *path);

protected:
    void SaveSettings();
    void GoToMainScreen();
    ScreenManager &screenManager;
    SettingsDevice &settings;
    SettingsStorage &settingsStorage;
    DisplayService &displayService;

private:
    SettingsScreenCommunication &settingsScreenCommunication;
};

#endif // SETTINGSSCREENBASE_H
