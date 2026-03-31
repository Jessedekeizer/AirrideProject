#include "SettingsScreenBase.h"

SettingsScreenBase::SettingsScreenBase(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService, EScreen screen, const char *path)
    : screenManager(screenManager), settingsScreenCommunication(settingsScreenCommunication), settings(settingsDevice), settingsStorage(settingsStorage), displayService(displayService), BaseScreen(screen, path) {
}

void SettingsScreenBase::SaveSettings() {
    settingsStorage.WriteSettings(settings);
    settingsScreenCommunication.SendSettings(settings);
    settingsStorage.ReadSettings(settings);
    screenManager.RequestScreen(EScreen::MAIN);
}

void SettingsScreenBase::GoToMainScreen() {
    screenManager.RequestScreen(EScreen::MAIN);
}
