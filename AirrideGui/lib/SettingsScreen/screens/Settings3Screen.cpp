#include "Settings3Screen.h"

#include "ESettingsScreenButtons.h"

Settings3Screen::Settings3Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication,
                                 SettingsDevice &settingsDevice, SettingsStorage &settingsStorage,
                                 DisplayService &displayService)
    : SettingsScreenBase(screenManager, settingsScreenCommunication, settingsDevice, settingsStorage, displayService,
                         EScreen::SETTINGS3, "/Settings3.png") {
    buttons.push_back(new PushButton(SETTINGSBTN_MAIN_X, SETTINGSBTN_MAIN_Y, SETTINGSBTN_MAIN_W, SETTINGSBTN_MAIN_H,
                                     MAIN_SCREEN,
                                     [this](Button &button) { SaveSettings(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_SAVE_X, SETTINGSBTN_SAVE_Y, SETTINGSBTN_SAVE_W, SETTINGSBTN_SAVE_H,
                                     SAVE,
                                     [this](Button &button) { GoToMainScreen(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_RIDE_SECONDS_SUBTRACT,
                                     [this](Button &button) { HandleAutoRideSecSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_RIDE_SECONDS_ADD,
                                     [this](Button &button) { HandleAutoRideSecAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_RIDE_ON,
                                     [this](Button &button) { HandleAutoRideOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_RIDE_OFF,
                                     [this](Button &button) { HandleAutoRideOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_PARK_SECONDS_SUBTRACT,
                                     [this](Button &button) { HandleAutoParkSecSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_PARK_SECONDS_ADD,
                                     [this](Button &button) { HandleAutoParkSecAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_PARK_ON,
                                     [this](Button &button) { HandleAutoParkOn(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W,
                                     SETTINGSBTN_ADJ_H, AUTO_PARK_OFF,
                                     [this](Button &button) { HandleAutoParkOff(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_LEFT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE,
                                     SETTINGSBTN_NAV_SIZE, SETTINGS2,
                                     [this](Button &button) { HandleSettings2(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_RIGHT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE,
                                     SETTINGSBTN_NAV_SIZE, SETTINGS4,
                                     [this](Button &button) { HandleSettings4(); }));
}

void Settings3Screen::OnSetup() {
    displayService.DrawText(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
    displayService.DrawBoolIcon(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
    displayService.DrawText(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
    displayService.DrawBoolIcon(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleAutoRideSecAdd() {
    settings.adjustValue(settings.autoRideSec, 0.1);
    displayService.DrawText(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings3Screen::HandleAutoRideSecSub() {
    settings.adjustValue(settings.autoRideSec, -0.1);
    displayService.DrawText(String(settings.autoRideSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings3Screen::HandleAutoRideOn() {
    if (settings.autoRide)
        return;

    settings.autoRide = true;
    displayService.DrawBoolIcon(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
}

void Settings3Screen::HandleAutoRideOff() {
    if (!settings.autoRide)
        return;

    settings.autoRide = false;
    displayService.DrawBoolIcon(settings.autoRide, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW2);
}

void Settings3Screen::HandleAutoParkSecAdd() {
    settings.adjustValue(settings.autoParkSec, 0.1);
    displayService.DrawText(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings3Screen::HandleAutoParkSecSub() {
    settings.adjustValue(settings.autoParkSec, -0.1);
    displayService.DrawText(String(settings.autoParkSec, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings3Screen::HandleAutoParkOn() {
    if (settings.autoPark)
        return;

    settings.autoPark = true;
    displayService.DrawBoolIcon(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleAutoParkOff() {
    if (!settings.autoPark)
        return;

    settings.autoPark = false;
    displayService.DrawBoolIcon(settings.autoPark, SETTINGS_BOOL_X, SETTINGS_BOOL_Y_ROW4);
}

void Settings3Screen::HandleSettings2() {
    screenManager.RequestScreen(EScreen::SETTINGS2);
}

void Settings3Screen::HandleSettings4() {
    screenManager.RequestScreen(EScreen::SETTINGS4);
}
