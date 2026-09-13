/**
 * @file Settings4Vars.cpp
 * @brief Settings page 4 - how long a park runs, and the three feature switches.
 * @note This page also carries the buttons into calibration and the update
 *       screens, but those only change screen, so they live with the rest of
 *       the navigation in SettingsScreenHandler.cpp.
 */

#include "SettingsVars.h"

SETTINGS_TEXT_VAR(park_duration_sec_text, parkDuration)
SETTINGS_STEP_ACTION(park_duration_sec_up, parkDuration, SETTINGS_STEP)
SETTINGS_STEP_ACTION(park_duration_sec_down, parkDuration, -SETTINGS_STEP)

SETTINGS_VALUE_VAR(logging, logging, bool, false)
SETTINGS_VALUE_VAR(machine_learning, machineLearning, bool, false)
SETTINGS_VALUE_VAR(bluetooth, bluetooth, bool, false)
