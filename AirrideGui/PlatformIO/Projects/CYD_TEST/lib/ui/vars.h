#ifndef EEZ_LVGL_UI_VARS_H
#define EEZ_LVGL_UI_VARS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enum declarations

// Flow global variables

enum FlowGlobalVariables {
    FLOW_GLOBAL_VARIABLE_NONE
};

// Native global variables

extern float get_var_front_pressure();
extern void set_var_front_pressure(float value);
extern float get_var_back_pressure();
extern void set_var_back_pressure(float value);
extern float get_var_front_max();
extern void set_var_front_max(float value);
extern float get_var_back_max();
extern void set_var_back_max(float value);
extern float get_var_ride_front();
extern void set_var_ride_front(float value);
extern float get_var_ride_back();
extern void set_var_ride_back(float value);
extern float get_var_front_up_x();
extern void set_var_front_up_x(float value);
extern float get_var_front_down_x();
extern void set_var_front_down_x(float value);
extern float get_var_back_up_x();
extern void set_var_back_up_x(float value);
extern float get_var_back_down_x();
extern void set_var_back_down_x(float value);
extern bool get_var_auto_ride();
extern void set_var_auto_ride(bool value);
extern bool get_var_auto_park();
extern void set_var_auto_park(bool value);
extern float get_var_auto_ride_sec();
extern void set_var_auto_ride_sec(float value);
extern float get_var_auto_park_sec();
extern void set_var_auto_park_sec(float value);
extern bool get_var_logging();
extern void set_var_logging(bool value);
extern bool get_var_machine_learning();
extern void set_var_machine_learning(bool value);
extern bool get_var_bluetooth();
extern void set_var_bluetooth(bool value);
extern float get_var_park_duration_sec();
extern void set_var_park_duration_sec(float value);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_VARS_H*/