#ifndef SETTINGSSCREENGEOMETRY_H
#define SETTINGSSCREENGEOMETRY_H

// shared geometry definitions for settings screens
// main/save buttons
#define SETTINGSBTN_MAIN_X 10
#define SETTINGSBTN_MAIN_Y 10
#define SETTINGSBTN_MAIN_W 50
#define SETTINGSBTN_MAIN_H 50

#define SETTINGSBTN_SAVE_X 260
#define SETTINGSBTN_SAVE_Y 10
#define SETTINGSBTN_SAVE_W SETTINGSBTN_MAIN_W
#define SETTINGSBTN_SAVE_H SETTINGSBTN_MAIN_H

// adjustment grid columns/rows and size
#define SETTINGSBTN_ADJ_COL1_X 126
#define SETTINGSBTN_ADJ_COL2_X 218
#define SETTINGSBTN_ADJ_ROW1_Y 73
#define SETTINGSBTN_ADJ_ROW2_Y 112
#define SETTINGSBTN_ADJ_ROW3_Y 159
#define SETTINGSBTN_ADJ_ROW4_Y 198
#define SETTINGSBTN_ADJ_W 35
#define SETTINGSBTN_ADJ_H 35

// navigation buttons at bottom corners
#define SETTINGSBTN_NAV_LEFT_X 10
#define SETTINGSBTN_NAV_RIGHT_X 275
#define SETTINGSBTN_NAV_BOTTOM_Y 198
#define SETTINGSBTN_NAV_SIZE 30

// special-purpose buttons
#define SETTINGSBTN_CALIB_X 269

// text display positions (shared by all settings screens)
#define SETTINGS_TEXT_X 180
#define SETTINGS_TEXT_ROW1_Y 82
#define SETTINGS_TEXT_ROW2_Y 122
#define SETTINGS_TEXT_ROW3_Y 167
#define SETTINGS_TEXT_ROW4_Y 207

// boolean display x coordinate (shared)
#define SETTINGS_BOOL_X 172
#define SETTINGS_BOOL_Y_ROW2 112
#define SETTINGS_BOOL_Y_ROW3 159
#define SETTINGS_BOOL_Y_ROW4 198

#endif // SETTINGSSCREENGEOMETRY_H
