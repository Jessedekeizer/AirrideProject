// Settings page 2 - the solenoid times, in seconds. One group per value:
// the label that shows it, and the two buttons that step it.
// See SettingsVars.h for the macros.

#include "SettingsVars.h"

SETTINGS_TEXT_VAR(front_up_xtext, frontUpX)
SETTINGS_STEP_ACTION(front_up_xup, frontUpX, SETTINGS_STEP)
SETTINGS_STEP_ACTION(front_up_xdown, frontUpX, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(front_down_xtext, frontDownX)
SETTINGS_STEP_ACTION(front_down_xup, frontDownX, SETTINGS_STEP)
SETTINGS_STEP_ACTION(front_down_xdown, frontDownX, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(back_up_xtext, backUpX)
SETTINGS_STEP_ACTION(back_up_xup, backUpX, SETTINGS_STEP)
SETTINGS_STEP_ACTION(back_up_xdown, backUpX, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(back_down_xtext, backDownX)
SETTINGS_STEP_ACTION(back_down_xup, backDownX, SETTINGS_STEP)
SETTINGS_STEP_ACTION(back_down_xdown, backDownX, -SETTINGS_STEP)
