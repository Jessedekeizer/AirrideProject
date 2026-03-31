#ifndef SETTINGS4SCREEN_H
#define SETTINGS4SCREEN_H

#include "IScreen.h"
#include "ScreenManager.h"
#include "SettingsScreenBase.h"
#include "SettingsScreenCommunication.h"

class Settings4Screen : public SettingsScreenBase {
public:
    Settings4Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService);

    void OnLoop() override {
    }

    void OnSetup() override;

private:
    void HandleSettings3();
    void HandleParkDurationAdd();
    void HandleParkDurationSub();
    void HandleLoggingOn();
    void HandleLoggingOff();
    void HandleMachineLearningOn();
    void HandleMachineLearningOff();
    void HandleBluetoothOn();
    void HandleBluetoothOff();
    void HandleCalibration();
};

#endif
