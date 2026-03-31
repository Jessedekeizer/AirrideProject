#ifndef SETTINGS3SCREEN_H
#define SETTINGS3SCREEN_H

#include "IScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenBase.h"
#include "SettingsScreenCommunication.h"

class Settings3Screen : public SettingsScreenBase {
public:
    Settings3Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService);

    void OnLoop() override {
    }

    void OnSetup() override;

private:
    void HandleSettings2();
    void HandleSettings4();
    void HandleAutoRideSecAdd();
    void HandleAutoRideSecSub();
    void HandleAutoRideOn();
    void HandleAutoRideOff();
    void HandleAutoParkSecAdd();
    void HandleAutoParkSecSub();
    void HandleAutoParkOn();
    void HandleAutoParkOff();
};

#endif
