#ifndef CALIBRATIONSCREEN_H
#define CALIBRATIONSCREEN_H

#include "BaseScreen.h"
#include "SettingsDevice.h"
#include "XPT2046_Bitbang.h"
#include "ECalibrationState.h"
#include "ScreenManager.h"
#include "DisplayService.h"
#include "SettingsStorage.h"

class CalibrationScreen : public BaseScreen {
public:
    CalibrationScreen(ScreenManager &screenManager, SettingsDevice &settings, SettingsStorage &settingsStorage, DisplayService &displayService, XPT2046_Bitbang &touchScreen);
    void OnLoop() override;
    void OnSetup() override;

private:
    void ChangeCalibrationState(ECalibrationState newState);
    void TopLeftCalibration();
    void BottomRightCalibration();
    void HandlePreCalibrationCountdown();
    void UpdateCountdown();
    void StartTopLeftCalibration();
    void HandleTopLeftCalibration();
    void HandleBottomRightCalibration();
    void StartBottomRightCalibration();
    void SaveCalibrationAndExit();
    void ClearScreen();
    void HandlePostCalibrationCountdown();

    void HandleTopLeftStart();
    void HandleBottomRightStart();
    int xmin = 0;
    int xmax = 0;
    int ymin = 0;
    int ymax = 0;

    int seconds = 3;

    ECalibrationState calibrationState;

    unsigned long TopLeftStartTime = 0;
    unsigned long BottomRightStartTime = 0;
    unsigned long preCalibrationTime = 0;
    unsigned long secondsTime = 0;

    long exitCountdownStart = 0;

    XPT2046_Bitbang &touchScreen;
    SettingsDevice &settings;
    ScreenManager &screenManager;
    SettingsStorage &settingsStorage;
    DisplayService &displayService;
};

#endif
