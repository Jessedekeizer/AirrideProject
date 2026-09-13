/**
 * @file Settings1Vars.cpp
 * @brief Settings page 1 - the pressure ceilings and the ride heights.
 * @note One group per value: the label that shows it, and the two buttons
 *       that step it. See SettingsVars.h for the macros.
 */

#include "SettingsVars.h"

SETTINGS_TEXT_VAR(front_max_text, frontMax)
SETTINGS_STEP_ACTION(front_max_up, frontMax, SETTINGS_STEP)
SETTINGS_STEP_ACTION(front_max_down, frontMax, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(back_max_text, backMax)
SETTINGS_STEP_ACTION(back_max_up, backMax, SETTINGS_STEP)
SETTINGS_STEP_ACTION(back_max_down, backMax, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(ride_front_text, rideFront)
SETTINGS_STEP_ACTION(ride_front_up, rideFront, SETTINGS_STEP)
SETTINGS_STEP_ACTION(ride_front_down, rideFront, -SETTINGS_STEP)

SETTINGS_TEXT_VAR(ride_back_text, rideBack)
SETTINGS_STEP_ACTION(ride_back_up, rideBack, SETTINGS_STEP)
SETTINGS_STEP_ACTION(ride_back_down, rideBack, -SETTINGS_STEP)
