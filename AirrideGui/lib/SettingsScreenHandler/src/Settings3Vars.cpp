/**
 * @file Settings3Vars.cpp
 * @brief Settings page 3 - auto ride and auto park, and how long each waits.
 * @note The two switches have no label or buttons of their own. The generated
 *       tick reads the getter to set the switch, and the switch calls the
 *       setter when it is toggled. See SettingsVars.h for the macros.
 */

#include "SettingsVars.h"

SETTINGS_VALUE_VAR(auto_ride, autoRide, bool, false)

SETTINGS_TEXT_VAR(auto_ride_sec_text, autoRideSec)
SETTINGS_STEP_ACTION(auto_ride_sec_up, autoRideSec, SETTINGS_STEP)
SETTINGS_STEP_ACTION(auto_ride_sec_down, autoRideSec, -SETTINGS_STEP)

SETTINGS_VALUE_VAR(auto_park, autoPark, bool, false)

SETTINGS_TEXT_VAR(auto_park_sec_text, autoParkSec)
SETTINGS_STEP_ACTION(auto_park_sec_up, autoParkSec, SETTINGS_STEP)
SETTINGS_STEP_ACTION(auto_park_sec_down, autoParkSec, -SETTINGS_STEP)
