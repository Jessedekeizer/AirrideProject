#include <string.h>

#include "screens.h"
#include "images.h"
#include "fonts.h"
#include "actions.h"
#include "vars.h"
#include "styles.h"
#include "ui.h"

#include <string.h>

objects_t objects;

//
// Event handlers
//

lv_obj_t *tick_value_change_obj;

static void event_handler_cb_settings3_obj8(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_auto_park(value);
        }
    }
}

static void event_handler_cb_settings3_obj9(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_auto_ride(value);
        }
    }
}

static void event_handler_cb_settings4_obj12(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_bluetooth(value);
        }
    }
}

static void event_handler_cb_settings4_obj13(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_logging(value);
        }
    }
}

static void event_handler_cb_settings4_obj14(lv_event_t *e) {
    lv_event_code_t event = lv_event_get_code(e);
    if (event == LV_EVENT_VALUE_CHANGED) {
        lv_obj_t *ta = lv_event_get_target_obj(e);
        if (tick_value_change_obj != ta) {
            bool value = lv_obj_has_state(ta, LV_STATE_CHECKED);
            set_var_machine_learning(value);
        }
    }
}

//
// Screens
//

void create_screen_main_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.main_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_main_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_main_screen_load_start, LV_EVENT_SCREEN_LOAD_START, (void *)0);
    lv_obj_add_event_cb(obj, action_main_screen_unload_start, LV_EVENT_SCREEN_UNLOAD_START, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // FrontUp
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.front_up = obj;
            lv_obj_set_pos(obj, 5, 5);
            lv_obj_set_size(obj, 50, 110);
            lv_obj_add_event_cb(obj, action_front_up_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_up_released, LV_EVENT_RELEASED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 17);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // FrontDown
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.front_down = obj;
            lv_obj_set_pos(obj, 5, 120);
            lv_obj_set_size(obj, 50, 110);
            lv_obj_add_event_cb(obj, action_front_down_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_down_released, LV_EVENT_RELEASED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 17);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // BackUp
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.back_up = obj;
            lv_obj_set_pos(obj, 265, 5);
            lv_obj_set_size(obj, 50, 110);
            lv_obj_add_event_cb(obj, action_back_up_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_up_released, LV_EVENT_RELEASED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 17);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // BackDown
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.back_down = obj;
            lv_obj_set_pos(obj, 265, 120);
            lv_obj_set_size(obj, 50, 110);
            lv_obj_add_event_cb(obj, action_back_down_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_down_released, LV_EVENT_RELEASED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 47);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 17);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // Ride
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.ride = obj;
            lv_obj_set_pos(obj, 75, 180);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_ride_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -1, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "R");
                }
            }
        }
        {
            // Park
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.park = obj;
            lv_obj_set_pos(obj, 195, 180);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_park_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "P");
                }
            }
        }
        {
            // Settings
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.settings = obj;
            lv_obj_set_pos(obj, 135, 180);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_main_screen_to_settings_select_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -2, 2);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_image_create(parent_obj);
            lv_obj_set_pos(obj, 64, 45);
            lv_obj_set_size(obj, 193, 80);
            lv_image_set_src(obj, &img_scirocco);
        }
        {
            // BackPressure
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.back_pressure = obj;
            lv_obj_set_pos(obj, 195, 120);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // BackPressureLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.back_pressure_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            // FrontPressure
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.front_pressure = obj;
            lv_obj_set_pos(obj, 75, 120);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // FrontPressureLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.front_pressure_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 113, 15);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Airride");
        }
    }
    
    tick_screen_main_screen();
}

void delete_screen_main_screen() {
    lv_obj_delete(objects.main_screen);
    objects.main_screen = 0;
    objects.front_up = 0;
    objects.front_down = 0;
    objects.back_up = 0;
    objects.back_down = 0;
    objects.ride = 0;
    objects.park = 0;
    objects.settings = 0;
    objects.back_pressure = 0;
    objects.back_pressure_label = 0;
    objects.front_pressure = 0;
    objects.front_pressure_label = 0;
}

void tick_screen_main_screen() {
    {
        const char *new_val = get_var_back_pressure_text();
        const char *cur_val = lv_label_get_text(objects.back_pressure_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.back_pressure_label;
            lv_label_set_text(objects.back_pressure_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_front_pressure_text();
        const char *cur_val = lv_label_get_text(objects.front_pressure_label);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = objects.front_pressure_label;
            lv_label_set_text(objects.front_pressure_label, new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_screen_update_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.update_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    {
        lv_obj_t *parent_obj = obj;
        {
            // ReturnToUpdateSelect
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.return_to_update_select = obj;
            lv_obj_set_pos(obj, 5, 5);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_update_to_update_select_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ReturnToUpdateSelectLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.return_to_update_select_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // UpdateScreenItemName
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.update_screen_item_name = obj;
            lv_obj_set_pos(obj, 120, 35);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "UpdateName");
        }
        {
            // UpdateScreenItemPercentage
            lv_obj_t *obj = lv_bar_create(parent_obj);
            objects.update_screen_item_percentage = obj;
            lv_obj_set_pos(obj, 18, 154);
            lv_obj_set_size(obj, 279, 48);
        }
        {
            // UpdateScreenItemStatus
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.update_screen_item_status = obj;
            lv_obj_set_pos(obj, 18, 129);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "Text");
        }
        {
            // UpdateFailedButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.update_failed_button = obj;
            lv_obj_set_pos(obj, 135, 70);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_update_to_update_select_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_color(obj, lv_color_lighten(lv_color_hex(0xff0000), 0), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_update_screen();
}

void delete_screen_update_screen() {
    lv_obj_delete(objects.update_screen);
    objects.update_screen = 0;
    objects.return_to_update_select = 0;
    objects.return_to_update_select_label = 0;
    objects.update_screen_item_name = 0;
    objects.update_screen_item_percentage = 0;
    objects.update_screen_item_status = 0;
    objects.update_failed_button = 0;
}

void tick_screen_update_screen() {
}

void create_screen_update_select_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.update_select_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_update_select_screen_loaded, LV_EVENT_SCREEN_LOADED, (void *)0);
    lv_obj_add_event_cb(obj, action_update_select_screen_load_start, LV_EVENT_SCREEN_LOAD_START, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            // UpdateToHome
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.update_to_home = obj;
            lv_obj_set_pos(obj, 5, 5);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_update_select_to_main_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // ReturnToSettingsLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.return_to_settings_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // PreviousItem
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.previous_item = obj;
            lv_obj_set_pos(obj, 10, 70);
            lv_obj_set_size(obj, 50, 160);
            lv_obj_add_event_cb(obj, action_previous_update_item, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // PreviousItemLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.previous_item_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // NextItem
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.next_item = obj;
            lv_obj_set_pos(obj, 260, 70);
            lv_obj_set_size(obj, 50, 160);
            lv_obj_add_event_cb(obj, action_next_update_item, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // NextItemLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    objects.next_item_label = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // UpdateItemContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.update_item_container = obj;
            lv_obj_set_pos(obj, 65, 70);
            lv_obj_set_size(obj, 190, 160);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(obj, LV_DIR_HOR);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // UpdateItem4
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.update_item4 = obj;
                    lv_obj_set_pos(obj, 760, 0);
                    lv_obj_set_size(obj, 190, 160);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_update_item(obj, 30);
                }
                {
                    // UpdateItem3
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.update_item3 = obj;
                    lv_obj_set_pos(obj, 570, 0);
                    lv_obj_set_size(obj, 190, 160);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_update_item(obj, 34);
                }
                {
                    // UpdateItem2
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.update_item2 = obj;
                    lv_obj_set_pos(obj, 380, 0);
                    lv_obj_set_size(obj, 190, 160);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_update_item(obj, 38);
                }
                {
                    // UpdateItem1
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.update_item1 = obj;
                    lv_obj_set_pos(obj, 190, 0);
                    lv_obj_set_size(obj, 190, 160);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_update_item(obj, 42);
                }
                {
                    // UpdateItem0
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.update_item0 = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 190, 160);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_update_item(obj, 46);
                }
            }
        }
        {
            // UpdateSelectLoading
            lv_obj_t *obj = lv_spinner_create(parent_obj);
            objects.update_select_loading = obj;
            lv_obj_set_pos(obj, 120, 80);
            lv_obj_set_size(obj, 80, 80);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }
    
    tick_screen_update_select_screen();
}

void delete_screen_update_select_screen() {
    lv_obj_delete(objects.update_select_screen);
    objects.update_select_screen = 0;
    objects.update_to_home = 0;
    objects.return_to_settings_label = 0;
    objects.previous_item = 0;
    objects.previous_item_label = 0;
    objects.next_item = 0;
    objects.next_item_label = 0;
    objects.update_item_container = 0;
    objects.update_item4 = 0;
    objects.update_item3 = 0;
    objects.update_item2 = 0;
    objects.update_item1 = 0;
    objects.update_item0 = 0;
    objects.update_select_loading = 0;
}

void tick_screen_update_select_screen() {
    tick_user_widget_update_item(30);
    tick_user_widget_update_item(34);
    tick_user_widget_update_item(38);
    tick_user_widget_update_item(42);
    tick_user_widget_update_item(46);
}

void create_screen_calibration_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.calibration_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_calibration_screen_load_started, LV_EVENT_SCREEN_LOAD_START, (void *)0);
    lv_obj_remove_flag(obj, LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_SNAPPABLE|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER);
    lv_obj_set_style_bg_color(obj, lv_color_hex(0x15171a), LV_PART_MAIN | LV_STATE_DEFAULT);
    {
        lv_obj_t *parent_obj = obj;
        {
            // TopLeftButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.top_left_button = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_top_left_button_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_top_left_button_pressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x15171a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, -13, -13);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_align(obj, LV_ALIGN_TOP_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // BottomRightButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            objects.bottom_right_button = obj;
            lv_obj_set_pos(obj, 270, 190);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_bottom_right_button_pressed, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_bottom_right_button_pressed, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
            lv_obj_set_style_bg_color(obj, lv_color_hex(0x15171a), LV_PART_MAIN | LV_STATE_DEFAULT);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 13, 13);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_BOTTOM_RIGHT, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // CalibrationTextLabel
            lv_obj_t *obj = lv_label_create(parent_obj);
            objects.calibration_text_label = obj;
            lv_obj_set_pos(obj, 0, 0);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "PlaceHolder");
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 265, 5);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_leave_calibration_screen, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
    
    tick_screen_calibration_screen();
}

void delete_screen_calibration_screen() {
    lv_obj_delete(objects.calibration_screen);
    objects.calibration_screen = 0;
    objects.top_left_button = 0;
    objects.bottom_right_button = 0;
    objects.calibration_text_label = 0;
}

void tick_screen_calibration_screen() {
}

void create_screen_settings_select_screen() {
    lv_obj_t *obj = lv_obj_create(0);
    objects.settings_select_screen = obj;
    lv_obj_set_pos(obj, 0, 0);
    lv_obj_set_size(obj, 320, 240);
    lv_obj_add_event_cb(obj, action_settings_select_load_start, LV_EVENT_SCREEN_LOAD_START, (void *)0);
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 5, 5);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_select_to_main_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 265, 5);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_save_settings_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 102, 15);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Settings");
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj0 = obj;
            lv_obj_set_pos(obj, 0, 60);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 320, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
        {
            // SettingsItemContainer
            lv_obj_t *obj = lv_obj_create(parent_obj);
            objects.settings_item_container = obj;
            lv_obj_set_pos(obj, 0, 61);
            lv_obj_set_size(obj, 320, 179);
            lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_radius(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_SCROLLABLE);
            lv_obj_set_scrollbar_mode(obj, LV_SCROLLBAR_MODE_OFF);
            lv_obj_set_scroll_dir(obj, LV_DIR_HOR);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // Settings1Item
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.settings1_item = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, 320, 179);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_settings1(obj, 56);
                }
                {
                    // Settings2Item
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.settings2_item = obj;
                    lv_obj_set_pos(obj, 320, 0);
                    lv_obj_set_size(obj, 320, 179);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_settings2(obj, 62);
                }
                {
                    // Settings3Item
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.settings3_item = obj;
                    lv_obj_set_pos(obj, 640, 0);
                    lv_obj_set_size(obj, 320, 179);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_settings3(obj, 69);
                }
                {
                    // Settings4Item
                    lv_obj_t *obj = lv_obj_create(parent_obj);
                    objects.settings4_item = obj;
                    lv_obj_set_pos(obj, 960, 0);
                    lv_obj_set_size(obj, 320, 179);
                    lv_obj_set_style_pad_left(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_top(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_right(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_pad_bottom(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_bg_opa(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_border_width(obj, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
                    create_user_widget_settings4(obj, 76);
                }
            }
        }
        {
            lv_obj_t *obj = lv_line_create(parent_obj);
            objects.obj1 = obj;
            lv_obj_set_pos(obj, 0, 146);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            static lv_point_precise_t line_points[] = {
                { 0, 0 },
                { 320, 0 }
            };
            lv_line_set_points(obj, line_points, 2);
            lv_obj_set_style_line_color(obj, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
        }
    }
    
    tick_screen_settings_select_screen();
}

void delete_screen_settings_select_screen() {
    lv_obj_delete(objects.settings_select_screen);
    objects.settings_select_screen = 0;
    objects.obj0 = 0;
    objects.settings_item_container = 0;
    objects.settings1_item = 0;
    objects.settings2_item = 0;
    objects.settings3_item = 0;
    objects.settings4_item = 0;
    objects.obj1 = 0;
}

void tick_screen_settings_select_screen() {
    tick_user_widget_settings1(56);
    tick_user_widget_settings2(62);
    tick_user_widget_settings3(69);
    tick_user_widget_settings4(76);
}

void create_user_widget_update_item(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            // UpdateItemUpdateButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 45, 80);
            lv_obj_set_size(obj, 100, 50);
            lv_obj_add_event_cb(obj, action_update_item_update_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    // UpdateItemUpdateButtonLabel
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "Update");
                }
            }
        }
        {
            // UpdateItemtext
            lv_obj_t *obj = lv_label_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 0, -30);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Text");
        }
    }
}

void tick_user_widget_update_item(int startWidgetIndex) {
    (void)startWidgetIndex;
}

void create_user_widget_settings1(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_max_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_max_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_max_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_max_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_max_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_max_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_max_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_max_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_ride_back_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_ride_back_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_ride_back_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_ride_back_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_ride_front_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_ride_front_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_ride_front_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_ride_front_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 7);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Ride");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 9, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Max");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 104, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "F");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 103, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "B");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 105, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "F");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 103, 134);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "B");
        }
        {
            // Settings1RightButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 266, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_right_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
}

void tick_user_widget_settings1(int startWidgetIndex) {
    (void)startWidgetIndex;
    {
        const char *new_val = get_var_front_max_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 1]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 1];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 1], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_ride_front_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 2]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 2];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 2], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_back_max_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 3]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 3];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 3], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_ride_back_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 4]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 4];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 4], new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_settings2(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_down_xdown, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_down_xdown, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 131);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_down_xup, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_down_xup, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_up_xdown, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_up_xdown, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_back_up_xup, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_back_up_xup, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_down_xdown, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_down_xdown, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 45);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_down_xup, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_down_xup, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_up_xdown, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_up_xdown, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_front_up_xup, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_front_up_xup, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Front");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Back");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 104, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "U");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 104, 47);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "D");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 104, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "U");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 103, 134);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "D");
        }
        {
            // Settings2RightButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 266, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_right_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // Settings2LeftButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 5, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_left_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
}

void tick_user_widget_settings2(int startWidgetIndex) {
    (void)startWidgetIndex;
    {
        const char *new_val = get_var_back_up_xtext();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 2]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 2];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 2], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_front_up_xtext();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 3]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 3];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 3], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_back_down_xtext();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 4]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 4];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 4], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_front_down_xtext();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 5]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 5];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 5], new_val);
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_settings3(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 5] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_auto_park_sec_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_auto_park_sec_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 91);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_auto_park_sec_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_auto_park_sec_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_auto_ride_sec_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_auto_ride_sec_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_auto_ride_sec_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_auto_ride_sec_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Ride");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Park");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 64, 134);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Auto");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 6, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Auto");
        }
        {
            lv_obj_t *obj = lv_switch_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 155, 131);
            lv_obj_set_size(obj, 70, 35);
            lv_obj_add_event_cb(obj, event_handler_cb_settings3_obj8, LV_EVENT_ALL, 0);
        }
        {
            lv_obj_t *obj = lv_switch_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 155, 45);
            lv_obj_set_size(obj, 70, 35);
            lv_obj_add_event_cb(obj, event_handler_cb_settings3_obj9, LV_EVENT_ALL, 0);
        }
        {
            // Settings3RightButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 266, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_right_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            // Settings3LeftButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 5, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_left_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
}

void tick_user_widget_settings3(int startWidgetIndex) {
    (void)startWidgetIndex;
    {
        const char *new_val = get_var_auto_park_sec_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 4]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 4];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 4], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        const char *new_val = get_var_auto_ride_sec_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 5]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 5];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 5], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_auto_park();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) {
                lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
            }
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_auto_ride();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 1];
            if (new_val) {
                lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
            }
            tick_value_change_obj = NULL;
        }
    }
}

void create_user_widget_settings4(lv_obj_t *parent_obj, int startWidgetIndex) {
    (void)startWidgetIndex;
    lv_obj_t *obj = parent_obj;
    {
        lv_obj_t *parent_obj = obj;
        {
            lv_obj_t *obj = lv_obj_create(parent_obj);
            lv_obj_set_pos(obj, 172, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_remove_flag(obj, LV_OBJ_FLAG_CLICKABLE|LV_OBJ_FLAG_CLICK_FOCUSABLE|LV_OBJ_FLAG_GESTURE_BUBBLE|LV_OBJ_FLAG_PRESS_LOCK|LV_OBJ_FLAG_SCROLLABLE|LV_OBJ_FLAG_SCROLL_CHAIN_HOR|LV_OBJ_FLAG_SCROLL_CHAIN_VER|LV_OBJ_FLAG_SCROLL_ELASTIC|LV_OBJ_FLAG_SCROLL_MOMENTUM|LV_OBJ_FLAG_SCROLL_WITH_ARROW|LV_OBJ_FLAG_SNAPPABLE);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    ((lv_obj_t **)&objects)[startWidgetIndex + 4] = obj;
                    lv_obj_set_pos(obj, -8, -5);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_text_align(obj, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 212, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_park_duration_sec_down, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_park_duration_sec_down, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 132, 5);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_park_duration_sec_up, LV_EVENT_PRESSED, (void *)0);
            lv_obj_add_event_cb(obj, action_park_duration_sec_up, LV_EVENT_LONG_PRESSED_REPEAT, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 7);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Park");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "M");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 7, 48);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Logging");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 67, 134);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "B");
        }
        {
            lv_obj_t *obj = lv_switch_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 0] = obj;
            lv_obj_set_pos(obj, 155, 131);
            lv_obj_set_size(obj, 70, 35);
            lv_obj_add_event_cb(obj, event_handler_cb_settings4_obj12, LV_EVENT_ALL, 0);
        }
        {
            lv_obj_t *obj = lv_switch_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 1] = obj;
            lv_obj_set_pos(obj, 155, 45);
            lv_obj_set_size(obj, 70, 35);
            lv_obj_add_event_cb(obj, event_handler_cb_settings4_obj13, LV_EVENT_ALL, 0);
        }
        {
            lv_obj_t *obj = lv_switch_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 2] = obj;
            lv_obj_set_pos(obj, 155, 91);
            lv_obj_set_size(obj, 70, 35);
            lv_obj_add_event_cb(obj, event_handler_cb_settings4_obj14, LV_EVENT_ALL, 0);
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 275, 46);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_settings_to_calibration_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 1, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 275, 92);
            lv_obj_set_size(obj, 35, 35);
            lv_obj_add_event_cb(obj, action_settings_to_update_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 1, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_button_create(parent_obj);
            lv_obj_set_pos(obj, 275, 132);
            lv_obj_set_size(obj, 35, 35);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 1);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 75, 20);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "duration");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 267, 8);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "Sec");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 82, 93);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
            lv_label_set_text_static(obj, "L");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 33, 105);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "achine");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 99, 105);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "earning");
        }
        {
            lv_obj_t *obj = lv_label_create(parent_obj);
            lv_obj_set_pos(obj, 88, 145);
            lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
            lv_label_set_text_static(obj, "luetooth");
        }
        {
            // Settings4LeftButton
            lv_obj_t *obj = lv_button_create(parent_obj);
            ((lv_obj_t **)&objects)[startWidgetIndex + 3] = obj;
            lv_obj_set_pos(obj, 5, 125);
            lv_obj_set_size(obj, 50, 50);
            lv_obj_add_event_cb(obj, action_settings_left_pressed, LV_EVENT_PRESSED, (void *)0);
            {
                lv_obj_t *parent_obj = obj;
                {
                    lv_obj_t *obj = lv_label_create(parent_obj);
                    lv_obj_set_pos(obj, 0, 0);
                    lv_obj_set_size(obj, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
                    lv_obj_set_style_align(obj, LV_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_obj_set_style_text_font(obj, &lv_font_montserrat_28, LV_PART_MAIN | LV_STATE_DEFAULT);
                    lv_label_set_text_static(obj, "");
                }
            }
        }
    }
}

void tick_user_widget_settings4(int startWidgetIndex) {
    (void)startWidgetIndex;
    {
        const char *new_val = get_var_park_duration_sec_text();
        const char *cur_val = lv_label_get_text(((lv_obj_t **)&objects)[startWidgetIndex + 4]);
        if (strcmp(new_val, cur_val) != 0) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 4];
            lv_label_set_text(((lv_obj_t **)&objects)[startWidgetIndex + 4], new_val);
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_bluetooth();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 0];
            if (new_val) {
                lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(((lv_obj_t **)&objects)[startWidgetIndex + 0], LV_STATE_CHECKED);
            }
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_logging();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 1];
            if (new_val) {
                lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(((lv_obj_t **)&objects)[startWidgetIndex + 1], LV_STATE_CHECKED);
            }
            tick_value_change_obj = NULL;
        }
    }
    {
        bool new_val = get_var_machine_learning();
        bool cur_val = lv_obj_has_state(((lv_obj_t **)&objects)[startWidgetIndex + 2], LV_STATE_CHECKED);
        if (new_val != cur_val) {
            tick_value_change_obj = ((lv_obj_t **)&objects)[startWidgetIndex + 2];
            if (new_val) {
                lv_obj_add_state(((lv_obj_t **)&objects)[startWidgetIndex + 2], LV_STATE_CHECKED);
            } else {
                lv_obj_remove_state(((lv_obj_t **)&objects)[startWidgetIndex + 2], LV_STATE_CHECKED);
            }
            tick_value_change_obj = NULL;
        }
    }
}

typedef void (*create_screen_func_t)();
create_screen_func_t create_screen_funcs[] = {
    create_screen_main_screen,
    create_screen_update_screen,
    create_screen_update_select_screen,
    create_screen_calibration_screen,
    create_screen_settings_select_screen,
};
void create_screen(int screen_index) {
    create_screen_funcs[screen_index]();
}
void create_screen_by_id(enum ScreensEnum screenId) {
    create_screen_funcs[screenId - 1]();
}

typedef void (*delete_screen_func_t)();
delete_screen_func_t delete_screen_funcs[] = {
    delete_screen_main_screen,
    delete_screen_update_screen,
    delete_screen_update_select_screen,
    delete_screen_calibration_screen,
    delete_screen_settings_select_screen,
};
void delete_screen(int screen_index) {
    delete_screen_funcs[screen_index]();
}
void delete_screen_by_id(enum ScreensEnum screenId) {
    delete_screen_funcs[screenId - 1]();
}

typedef void (*tick_screen_func_t)();
tick_screen_func_t tick_screen_funcs[] = {
    tick_screen_main_screen,
    tick_screen_update_screen,
    tick_screen_update_select_screen,
    tick_screen_calibration_screen,
    tick_screen_settings_select_screen,
};
void tick_screen(int screen_index) {
    if (screen_index >= 0 && screen_index < 5) {
        tick_screen_funcs[screen_index]();
    }
}
void tick_screen_by_id(enum ScreensEnum screenId) {
    tick_screen(screenId - 1);
}

//
// Fonts
//

ext_font_desc_t fonts[] = {
#if LV_FONT_MONTSERRAT_8
    { "MONTSERRAT_8", &lv_font_montserrat_8 },
#endif
#if LV_FONT_MONTSERRAT_10
    { "MONTSERRAT_10", &lv_font_montserrat_10 },
#endif
#if LV_FONT_MONTSERRAT_12
    { "MONTSERRAT_12", &lv_font_montserrat_12 },
#endif
#if LV_FONT_MONTSERRAT_14
    { "MONTSERRAT_14", &lv_font_montserrat_14 },
#endif
#if LV_FONT_MONTSERRAT_16
    { "MONTSERRAT_16", &lv_font_montserrat_16 },
#endif
#if LV_FONT_MONTSERRAT_18
    { "MONTSERRAT_18", &lv_font_montserrat_18 },
#endif
#if LV_FONT_MONTSERRAT_20
    { "MONTSERRAT_20", &lv_font_montserrat_20 },
#endif
#if LV_FONT_MONTSERRAT_22
    { "MONTSERRAT_22", &lv_font_montserrat_22 },
#endif
#if LV_FONT_MONTSERRAT_24
    { "MONTSERRAT_24", &lv_font_montserrat_24 },
#endif
#if LV_FONT_MONTSERRAT_26
    { "MONTSERRAT_26", &lv_font_montserrat_26 },
#endif
#if LV_FONT_MONTSERRAT_28
    { "MONTSERRAT_28", &lv_font_montserrat_28 },
#endif
#if LV_FONT_MONTSERRAT_30
    { "MONTSERRAT_30", &lv_font_montserrat_30 },
#endif
#if LV_FONT_MONTSERRAT_32
    { "MONTSERRAT_32", &lv_font_montserrat_32 },
#endif
#if LV_FONT_MONTSERRAT_34
    { "MONTSERRAT_34", &lv_font_montserrat_34 },
#endif
#if LV_FONT_MONTSERRAT_36
    { "MONTSERRAT_36", &lv_font_montserrat_36 },
#endif
#if LV_FONT_MONTSERRAT_38
    { "MONTSERRAT_38", &lv_font_montserrat_38 },
#endif
#if LV_FONT_MONTSERRAT_40
    { "MONTSERRAT_40", &lv_font_montserrat_40 },
#endif
#if LV_FONT_MONTSERRAT_42
    { "MONTSERRAT_42", &lv_font_montserrat_42 },
#endif
#if LV_FONT_MONTSERRAT_44
    { "MONTSERRAT_44", &lv_font_montserrat_44 },
#endif
#if LV_FONT_MONTSERRAT_46
    { "MONTSERRAT_46", &lv_font_montserrat_46 },
#endif
#if LV_FONT_MONTSERRAT_48
    { "MONTSERRAT_48", &lv_font_montserrat_48 },
#endif
};

//
// Color themes
//

uint32_t active_theme_index = 0;

//
//
//

void create_screens() {

// Set default LVGL theme
    lv_display_t *dispp = lv_display_get_default();
    lv_theme_t *theme = lv_theme_default_init(dispp, lv_palette_main(LV_PALETTE_BLUE), lv_palette_main(LV_PALETTE_RED), true, LV_FONT_DEFAULT);
    lv_display_set_theme(dispp, theme);
    
    // Initialize screens
    // Create screens
    create_screen_main_screen();
    create_screen_update_screen();
    create_screen_update_select_screen();
    create_screen_calibration_screen();
    create_screen_settings_select_screen();
}