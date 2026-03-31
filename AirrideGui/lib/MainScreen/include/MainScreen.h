#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "BaseScreen.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "ScreenManager.h"
#include "SettingsDevice.h"
#include "LogStorage.h"
#include "Timer.h"

class MainScreen : public BaseScreen
{
public:
    MainScreen(MainScreenData &mainScreenData, MainScreenCommunication &mainScreenCommunication, ScreenManager &screenManager, SettingsDevice &settings, DisplayService &displayService, LogStorage &logStorage);
    void OnLoop() override;
    void OnSetup() override;
    ~MainScreen() override;

private:
    void GoToSettings1();
    void HandlePushButton(Button &sender);
    void AutoStartRide();
    void HandleToggleButton(Button &sender);
    void AddRideTimer();
    MainScreenData &mainScreenData;
    MainScreenCommunication &mainScreenCommunication;
    ScreenManager &screenManager;
    SettingsDevice &settings;
    DisplayService &displayService;
    LogStorage &logStorage;
    float frontPressure;
    float backPressure;
    bool abortAutoRide = false;
    Timer *autoRideTimer = nullptr;
};

#endif
