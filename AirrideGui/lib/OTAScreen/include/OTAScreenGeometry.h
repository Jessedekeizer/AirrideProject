#ifndef OTA_SCREEN_GEOMETRY_H
#define OTA_SCREEN_GEOMETRY_H

// Screen
#define OTA_SCR_W  320
#define OTA_SCR_H  177
#define SCREEN_PATH "/OTAScreen.png"

// Scanning screen
#define OTA_SCAN_LINE1_X        95
#define OTA_SCAN_LINE1_Y        90
#define OTA_SCAN_LINE2_X        113
#define OTA_SCAN_LINE2_Y        110

// Select screen
#define OTA_SELECT_TITLE_X      85
#define OTA_SELECT_TITLE_Y      63
#define OTA_SELECT_NONE_X       57
#define OTA_SELECT_NONE_Y       110

#define OTA_NODE_NAME_X         80
#define OTA_NODE_NAME_Y         105

#define OTA_SELECT_COUNTER_X    140
#define OTA_SELECT_COUNTER_Y    150

// Left / right arrow buttons
#define OTA_BTN_LEFT_X          5
#define OTA_BTN_LEFT_Y          88
#define OTA_BTN_LEFT_W          50
#define OTA_BTN_LEFT_H          50

#define OTA_BTN_RIGHT_X         265
#define OTA_BTN_RIGHT_Y         88
#define OTA_BTN_RIGHT_W         50
#define OTA_BTN_RIGHT_H         50

// START / RETRY button (bottom-left)
#define OTA_BTN_START_X         85
#define OTA_BTN_START_Y         185
#define OTA_BTN_START_W         130
#define OTA_BTN_START_H         45
#define OTA_BTN_START_LABEL_X   100
#define OTA_BTN_START_LABEL_Y   200

// BACK button (bottom-right)
#define OTA_BTN_BACK_X          10
#define OTA_BTN_BACK_Y          10
#define OTA_BTN_BACK_W          50
#define OTA_BTN_BACK_H          50

// Updating screen
#define OTA_UPDATE_NODE_X       20
#define OTA_UPDATE_NODE_Y       63

#define OTA_UPDATE_STATUS_X     20
#define OTA_UPDATE_STATUS_Y     100

#define OTA_PROGRESS_X          20
#define OTA_PROGRESS_Y          115
#define OTA_PROGRESS_W          280
#define OTA_PROGRESS_H          28

#define OTA_PROGRESS_PCT_X      148
#define OTA_PROGRESS_PCT_Y      152

// Error screen
#define OTA_ERROR_X             82
#define OTA_ERROR_Y             90

// Done screen
#define OTA_DONE_X              52
#define OTA_DONE_Y              100

#endif // OTA_SCREEN_GEOMETRY_H
