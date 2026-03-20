#include "Settings1Screen.h"

#include "ESettingsScreenButtons.h"

Settings1Screen::Settings1Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService)
    : SettingsScreenBase(screenManager, settingsScreenCommunication, settingsDevice, settingsStorage, displayService, EScreen::SETTINGS1, "/Settings1.png") {
    buttons.push_back(new PushButton(SETTINGSBTN_MAIN_X, SETTINGSBTN_MAIN_Y, SETTINGSBTN_MAIN_W, SETTINGSBTN_MAIN_H, MAIN_SCREEN,
                                     [this](Button &button) { GoToMainScreen(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_SAVE_X, SETTINGSBTN_SAVE_Y, SETTINGSBTN_SAVE_W, SETTINGSBTN_SAVE_H, SAVE,
                                     [this](Button &button) { SaveSettings(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, RIDE_FRONT_SUBTRACT,
                                     [this](Button &button) { HandleRideFrontSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, RIDE_FRONT_ADD,
                                     [this](Button &button) { HandleRideFrontAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, RIDE_BACK_SUBTRACT,
                                     [this](Button &button) { HandleRideBackSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, RIDE_BACK_ADD,
                                     [this](Button &button) { HandleRideBackAdd(); }));
    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MAX_FRONT_SUBTRACT,
                                     [this](Button &button) { HandleMaxFrontSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MAX_FRONT_ADD,
                                     [this](Button &button) { HandleMaxFrontAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MAX_BACK_SUBTRACT,
                                     [this](Button &button) { HandleMaxBackSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, MAX_BACK_ADD,
                                     [this](Button &button) { HandleMaxBackAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_RIGHT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS2,
                                     [this](Button &button) { HandleSettings2(); }));
}

void Settings1Screen::OnSetup() {
    displayService.DrawText(String(settings.rideFront, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
    displayService.DrawText(String(settings.rideBack, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
    displayService.DrawText(String(settings.frontMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
    displayService.DrawText(String(settings.backMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings1Screen::HandleRideFrontAdd() {
    settings.adjustValue(settings.rideFront, 0.1);
    displayService.DrawText(String(settings.rideFront, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings1Screen::HandleRideFrontSub() {
    settings.adjustValue(settings.rideFront, -0.1);
    displayService.DrawText(String(settings.rideFront, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings1Screen::HandleRideBackAdd() {
    settings.adjustValue(settings.rideBack, 0.1);
    displayService.DrawText(String(settings.rideBack, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings1Screen::HandleRideBackSub() {
    settings.adjustValue(settings.rideBack, -0.1);
    displayService.DrawText(String(settings.rideBack, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings1Screen::HandleMaxFrontAdd() {
    settings.adjustValue(settings.frontMax, 0.1);
    displayService.DrawText(String(settings.frontMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings1Screen::HandleMaxFrontSub() {
    settings.adjustValue(settings.frontMax, -0.1);
    displayService.DrawText(String(settings.frontMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings1Screen::HandleMaxBackAdd() {
    settings.adjustValue(settings.backMax, 0.1);
    displayService.DrawText(String(settings.backMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings1Screen::HandleMaxBackSub() {
    settings.adjustValue(settings.backMax, -0.1);
    displayService.DrawText(String(settings.backMax, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings1Screen::HandleSettings2() {
    screenManager.RequestScreen(EScreen::SETTINGS2);
}
