#ifndef EEZ_LVGL_UI_EVENTS_H
#define EEZ_LVGL_UI_EVENTS_H

#include <Lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

extern void action_front_up_pressed(lv_event_t * e);
extern void action_front_up_released(lv_event_t * e);
extern void action_front_down_pressed(lv_event_t * e);
extern void action_front_down_released(lv_event_t * e);
extern void action_back_up_pressed(lv_event_t * e);
extern void action_back_up_released(lv_event_t * e);
extern void action_back_down_pressed(lv_event_t * e);
extern void action_back_down_released(lv_event_t * e);
extern void action_update_to_update_select_pressed(lv_event_t * e);
extern void action_next_update_item(lv_event_t * e);
extern void action_previous_update_item(lv_event_t * e);
extern void action_update_select_screen_loaded(lv_event_t * e);
extern void action_update_item_update_pressed(lv_event_t * e);
extern void action_main_screen_loaded(lv_event_t * e);
extern void action_update_select_to_main_pressed(lv_event_t * e);
extern void action_save_settings_pressed(lv_event_t * e);
extern void action_settings_to_update_pressed(lv_event_t * e);
extern void action_park_pressed(lv_event_t * e);
extern void action_ride_pressed(lv_event_t * e);
extern void action_main_screen_to_settings_select_pressed(lv_event_t * e);
extern void action_settings_left_pressed(lv_event_t * e);
extern void action_settings_right_pressed(lv_event_t * e);
extern void action_settings_select_load_start(lv_event_t * e);
extern void action_top_left_button_pressed(lv_event_t * e);
extern void action_bottom_right_button_pressed(lv_event_t * e);
extern void action_calibration_screen_load_started(lv_event_t * e);
extern void action_leave_calibration_screen(lv_event_t * e);

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_EVENTS_H*/