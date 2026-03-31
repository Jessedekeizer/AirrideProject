#ifndef SETTINGS1SCREEN_H
#define SETTINGS1SCREEN_H

#include "ScreenManager.h"
#include "SettingsScreenBase.h"
#include "SettingsScreenCommunication.h"

class Settings1Screen : public SettingsScreenBase {
public:
    Settings1Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService);

    void OnLoop() override {
    };
    void OnSetup() override;

private:
    void HandleRideFrontAdd();
    void HandleRideFrontSub();
    void HandleRideBackAdd();
    void HandleRideBackSub();
    void HandleMaxFrontAdd();
    void HandleMaxFrontSub();
    void HandleMaxBackAdd();
    void HandleMaxBackSub();
    void HandleSettings2();
};

#endif
