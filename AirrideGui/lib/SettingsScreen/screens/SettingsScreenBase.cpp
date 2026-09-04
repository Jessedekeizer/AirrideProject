#include "SettingsScreenBase.h"

SettingsScreenBase::SettingsScreenBase(ScreenManager &screenManager, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService, EScreen screen, const char *path)
    : screenManager(screenManager), settings(settingsDevice), settingsStorage(settingsStorage), displayService(displayService), BaseScreen(screen, path) {
}

void SettingsScreenBase::SaveSettings() {
    settingsStorage.WriteSettings(settings);
    settingsStorage.ReadSettings(settings);
    screenManager.RequestScreen(EScreen::MAIN);
}

void SettingsScreenBase::GoToMainScreen() {
    screenManager.RequestScreen(EScreen::MAIN);
}
