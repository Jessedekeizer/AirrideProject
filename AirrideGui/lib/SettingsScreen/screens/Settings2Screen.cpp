#include "Settings2Screen.h"

#include "ESettingsScreenButtons.h"
#include "Logger.h"

Settings2Screen::Settings2Screen(ScreenManager &screenManager, SettingsScreenCommunication &settingsScreenCommunication, SettingsDevice &settingsDevice, SettingsStorage &settingsStorage, DisplayService &displayService)
    : SettingsScreenBase(screenManager, settingsScreenCommunication, settingsDevice, settingsStorage, displayService, EScreen::SETTINGS2, "/Settings2.png") {
    buttons.push_back(new PushButton(SETTINGSBTN_MAIN_X, SETTINGSBTN_MAIN_Y, SETTINGSBTN_MAIN_W, SETTINGSBTN_MAIN_H, MAIN_SCREEN,
                                     [this](Button &button) { GoToMainScreen(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_SAVE_X, SETTINGSBTN_SAVE_Y, SETTINGSBTN_SAVE_W, SETTINGSBTN_SAVE_H, SAVE,
                                     [this](Button &button) { SaveSettings(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, FRONT_UP_FACTOR_SUBTRACT,
                                     [this](Button &button) { HandleFrontUpFactorSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW1_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, FRONT_UP_FACTOR_ADD,
                                     [this](Button &button) { HandleFrontUpFactorAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, FRONT_DOWN_FACTOR_SUBTRACT,
                                     [this](Button &button) { HandleFrontDownFactorSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW2_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, FRONT_DOWN_FACTOR_ADD,
                                     [this](Button &button) { HandleFrontDownFactorAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BACK_UP_FACTOR_SUBTRACT,
                                     [this](Button &button) { HandleBackUpFactorSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW3_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BACK_UP_FACTOR_ADD,
                                     [this](Button &button) { HandleBackUpFactorAdd(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL1_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BACK_DOWN_FACTOR_SUBTRACT,
                                     [this](Button &button) { HandleBackDownFactorSub(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_ADJ_COL2_X, SETTINGSBTN_ADJ_ROW4_Y, SETTINGSBTN_ADJ_W, SETTINGSBTN_ADJ_H, BACK_DOWN_FACTOR_ADD,
                                     [this](Button &button) { HandleBackDownFactorAdd(); }));
    buttons.push_back(new PushButton(SETTINGSBTN_NAV_LEFT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS1,
                                     [this](Button &button) { HandleSettings1(); }));

    buttons.push_back(new PushButton(SETTINGSBTN_NAV_RIGHT_X, SETTINGSBTN_NAV_BOTTOM_Y, SETTINGSBTN_NAV_SIZE, SETTINGSBTN_NAV_SIZE, SETTINGS3,
                                     [this](Button &button) { HandleSettings3(); }));
}

void Settings2Screen::OnSetup() {
    displayService.DrawText(String(settings.frontUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
    displayService.DrawText(String(settings.frontDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
    displayService.DrawText(String(settings.backUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
    displayService.DrawText(String(settings.backDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings2Screen::HandleFrontUpFactorAdd() {
    settings.adjustValue(settings.frontUpX, 0.1);
    displayService.DrawText(String(settings.frontUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings2Screen::HandleFrontUpFactorSub() {
    settings.adjustValue(settings.frontUpX, -0.1);
    displayService.DrawText(String(settings.frontUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW1_Y);
}

void Settings2Screen::HandleFrontDownFactorAdd() {
    settings.adjustValue(settings.frontDownX, 0.1);
    displayService.DrawText(String(settings.frontDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings2Screen::HandleFrontDownFactorSub() {
    settings.adjustValue(settings.frontDownX, -0.1);
    displayService.DrawText(String(settings.frontDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW2_Y);
}

void Settings2Screen::HandleBackUpFactorAdd() {
    settings.adjustValue(settings.backUpX, 0.1);
    displayService.DrawText(String(settings.backUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings2Screen::HandleBackUpFactorSub() {
    settings.adjustValue(settings.backUpX, -0.1);
    displayService.DrawText(String(settings.backUpX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW3_Y);
}

void Settings2Screen::HandleBackDownFactorAdd() {
    settings.adjustValue(settings.backDownX, 0.1);
    displayService.DrawText(String(settings.backDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings2Screen::HandleBackDownFactorSub() {
    settings.adjustValue(settings.backDownX, -0.1);
    displayService.DrawText(String(settings.backDownX, 1), SETTINGS_TEXT_X, SETTINGS_TEXT_ROW4_Y);
}

void Settings2Screen::HandleSettings1() {
    screenManager.RequestScreen(EScreen::SETTINGS1);
}

void Settings2Screen::HandleSettings3() {
    screenManager.RequestScreen(EScreen::SETTINGS3);
}
