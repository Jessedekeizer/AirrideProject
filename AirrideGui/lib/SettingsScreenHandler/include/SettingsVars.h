#ifndef SETTINGSVARS_H
#define SETTINGSVARS_H

#include <lvgl.h>
#include <stddef.h>
#include <stdio.h>

#include "SettingsDevice.h"
#include "SettingsScreenHandler.h"

/**
 * @file SettingsVars.h
 * @brief The shims behind one settings page, as three macros.
 *
 * Every settings page is the same shape: a handful of values out of the one
 * shared SettingsDevice, each bound to a label, a pair of up and down buttons,
 * or a switch. The generated code wants a differently named function per
 * value, so the pages are written as macro calls - one group of lines per
 * value - in Settings1Vars.cpp through Settings4Vars.cpp.
 *
 * The names passed in have to match what EEZ Studio emits exactly. It splits
 * a capital into an underscore only when a lowercase letter precedes it, so
 * FrontUpXText becomes front_up_xtext rather than front_up_x_text.
 *
 * Only what the generated code actually calls is defined here. vars.h still
 * declares a getter and a setter for every variable in the project, but an
 * unused declaration costs nothing, and binding a widget to one of them later
 * fails loudly at link time rather than quietly doing the wrong thing.
 */

/** Room for "%.1f" of any value here, plus a sign and a terminator. */
static constexpr size_t SETTINGS_TEXT_WIDTH = 12;

/** How far one press of an up or down button moves a value. */
static constexpr float SETTINGS_STEP = 0.1f;

/**
 * @brief The settings every page reads and writes.
 * @return The shared struct, or null before a handler exists.
 */
SettingsDevice *ActiveSettings();

/**
 * @brief Define the read and write shims for one value.
 * @param name The generated function name stem.
 * @param member The field in the settings.
 * @param type The type the generated header declares.
 * @param fallback What to read back before a handler exists.
 * @note Only the switches want this. The generated tick calls the getter to
 *       decide whether the switch needs redrawing, and the switch calls the
 *       setter when it is toggled. A value the labels merely show wants
 *       SETTINGS_TEXT_VAR instead - nothing reads those as numbers.
 */
#define SETTINGS_VALUE_VAR(name, member, type, fallback)                             \
    extern "C" type get_var_##name() {                                               \
        SettingsDevice *s = ActiveSettings();                                        \
        return s != nullptr ? s->member : fallback;                                  \
    }                                                                                \
    extern "C" void set_var_##name(type value) {                                     \
        SettingsDevice *s = ActiveSettings();                                        \
        if (s != nullptr) {                                                          \
            s->member = value;                                                       \
            SettingsScreenHandler::MarkDirty();                                      \
        }                                                                            \
    }

/**
 * @brief Define the shim for the label that shows one value.
 * @param name The generated getter name, spelled exactly as EEZ emits it.
 * @param member The field in the settings.
 * @note The labels take text, so the tenths formatting the flow engine used
 *       to do lives here. The buffer is static because LVGL only copies out
 *       of it after the tick has returned. Read only - the buttons write,
 *       through SETTINGS_STEP_ACTION.
 */
#define SETTINGS_TEXT_VAR(name, member)                                              \
    extern "C" const char *get_var_##name() {                                        \
        static char buffer[SETTINGS_TEXT_WIDTH];                                     \
        SettingsDevice *s = ActiveSettings();                                        \
        snprintf(buffer, sizeof(buffer), "%.1f", s != nullptr ? s->member : 0.0f);   \
        return buffer;                                                               \
    }

/**
 * @brief Define one step action behind a settings up or down button.
 * @param name The generated function name stem.
 * @param member The field in the settings.
 * @param delta How far one press moves it.
 * @note AdjustValue clamps at zero, which is what the flow expression
 *       `X < 0.1 ? 0.0 : X - 0.1` did. The buttons repeat on
 *       LONG_PRESSED_REPEAT, so this runs once per repeat as well.
 */
#define SETTINGS_STEP_ACTION(name, member, delta)                                    \
    extern "C" void action_##name(lv_event_t *e) {                                   \
        (void) e;                                                                    \
        SettingsDevice *s = ActiveSettings();                                        \
        if (s != nullptr) {                                                          \
            s->AdjustValue(s->member, delta);                                        \
            SettingsScreenHandler::MarkDirty();                                      \
        }                                                                            \
    }

#endif
