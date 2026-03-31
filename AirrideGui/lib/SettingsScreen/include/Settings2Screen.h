#ifndef SETTINGS2SCREEN_H
#define SETTINGS2SCREEN_H

#include "ScreenManager.h"
#include "SettingsScreenBase.h"

class Settings2Screen : public SettingsScreenBase {
public:
    Settings2Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService);

    void OnLoop() override {
    }

    void OnSetup() override;

private:
    void HandleSettings1();
    void HandleSettings3();
    void HandleFrontUpFactorAdd();
    void HandleFrontUpFactorSub();
    void HandleFrontDownFactorAdd();
    void HandleFrontDownFactorSub();
    void HandleBackUpFactorAdd();
    void HandleBackUpFactorSub();
    void HandleBackDownFactorAdd();
    void HandleBackDownFactorSub();
};

#endif
