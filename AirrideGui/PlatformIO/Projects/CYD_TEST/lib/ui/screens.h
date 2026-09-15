#ifndef EEZ_LVGL_UI_SCREENS_H
#define EEZ_LVGL_UI_SCREENS_H

#include <Lvgl.h>

#ifdef __cplusplus
extern "C" {
#endif

// Screens

enum ScreensEnum {
    _SCREEN_ID_FIRST = 1,
    SCREEN_ID_MAIN_SCREEN = 1,
    SCREEN_ID_UPDATE_SCREEN = 2,
    SCREEN_ID_UPDATE_SELECT_SCREEN = 3,
    SCREEN_ID_CALIBRATION_SCREEN = 4,
    SCREEN_ID_SETTINGS_SELECT_SCREEN = 5,
    _SCREEN_ID_LAST = 5
};

typedef struct _objects_t {
    lv_obj_t *main_screen;
    lv_obj_t *update_screen;
    lv_obj_t *update_select_screen;
    lv_obj_t *calibration_screen;
    lv_obj_t *settings_select_screen;
    lv_obj_t *update_item4;
    lv_obj_t *update_item4__update_item_update_button;
    lv_obj_t *update_item4__update_item_update_button_label;
    lv_obj_t *update_item4__update_itemtext;
    lv_obj_t *update_item3;
    lv_obj_t *update_item3__update_item_update_button;
    lv_obj_t *update_item3__update_item_update_button_label;
    lv_obj_t *update_item3__update_itemtext;
    lv_obj_t *update_item2;
    lv_obj_t *update_item2__update_item_update_button;
    lv_obj_t *update_item2__update_item_update_button_label;
    lv_obj_t *update_item2__update_itemtext;
    lv_obj_t *update_item1;
    lv_obj_t *update_item1__update_item_update_button;
    lv_obj_t *update_item1__update_item_update_button_label;
    lv_obj_t *update_item1__update_itemtext;
    lv_obj_t *update_item0;
    lv_obj_t *update_item0__update_item_update_button;
    lv_obj_t *update_item0__update_item_update_button_label;
    lv_obj_t *update_item0__update_itemtext;
    lv_obj_t *settings1_item;
    lv_obj_t *settings1_item__obj0;
    lv_obj_t *settings1_item__obj1;
    lv_obj_t *settings1_item__obj2;
    lv_obj_t *settings1_item__obj3;
    lv_obj_t *settings1_item__obj4;
    lv_obj_t *settings1_item__obj5;
    lv_obj_t *settings1_item__obj6;
    lv_obj_t *settings1_item__obj7;
    lv_obj_t *settings1_item__settings1_right_button;
    lv_obj_t *settings1_item__obj8;
    lv_obj_t *settings1_item__obj9;
    lv_obj_t *settings1_item__obj10;
    lv_obj_t *settings1_item__obj11;
    lv_obj_t *settings1_item__obj12;
    lv_obj_t *settings1_item__obj13;
    lv_obj_t *settings1_item__obj14;
    lv_obj_t *settings1_item__obj15;
    lv_obj_t *settings1_item__obj16;
    lv_obj_t *settings1_item__obj17;
    lv_obj_t *settings1_item__obj18;
    lv_obj_t *settings1_item__obj19;
    lv_obj_t *settings1_item__obj20;
    lv_obj_t *settings2_item;
    lv_obj_t *settings2_item__obj21;
    lv_obj_t *settings2_item__obj22;
    lv_obj_t *settings2_item__obj23;
    lv_obj_t *settings2_item__obj24;
    lv_obj_t *settings2_item__obj25;
    lv_obj_t *settings2_item__obj26;
    lv_obj_t *settings2_item__obj27;
    lv_obj_t *settings2_item__obj28;
    lv_obj_t *settings2_item__settings2_right_button;
    lv_obj_t *settings2_item__settings2_left_button;
    lv_obj_t *settings2_item__obj29;
    lv_obj_t *settings2_item__obj30;
    lv_obj_t *settings2_item__obj31;
    lv_obj_t *settings2_item__obj32;
    lv_obj_t *settings2_item__obj33;
    lv_obj_t *settings2_item__obj34;
    lv_obj_t *settings2_item__obj35;
    lv_obj_t *settings2_item__obj36;
    lv_obj_t *settings2_item__obj37;
    lv_obj_t *settings2_item__obj38;
    lv_obj_t *settings2_item__obj39;
    lv_obj_t *settings2_item__obj40;
    lv_obj_t *settings2_item__obj41;
    lv_obj_t *settings2_item__obj42;
    lv_obj_t *settings3_item;
    lv_obj_t *settings3_item__obj43;
    lv_obj_t *settings3_item__obj44;
    lv_obj_t *settings3_item__obj45;
    lv_obj_t *settings3_item__obj46;
    lv_obj_t *settings3_item__obj47;
    lv_obj_t *settings3_item__obj48;
    lv_obj_t *settings3_item__settings3_right_button;
    lv_obj_t *settings3_item__settings3_left_button;
    lv_obj_t *settings3_item__obj49;
    lv_obj_t *settings3_item__obj50;
    lv_obj_t *settings3_item__obj51;
    lv_obj_t *settings3_item__obj52;
    lv_obj_t *settings3_item__obj53;
    lv_obj_t *settings3_item__obj54;
    lv_obj_t *settings3_item__obj55;
    lv_obj_t *settings3_item__obj56;
    lv_obj_t *settings4_item;
    lv_obj_t *settings4_item__obj57;
    lv_obj_t *settings4_item__obj58;
    lv_obj_t *settings4_item__obj59;
    lv_obj_t *settings4_item__obj60;
    lv_obj_t *settings4_item__obj61;
    lv_obj_t *settings4_item__obj62;
    lv_obj_t *settings4_item__obj63;
    lv_obj_t *settings4_item__settings4_left_button;
    lv_obj_t *settings4_item__obj64;
    lv_obj_t *settings4_item__obj65;
    lv_obj_t *settings4_item__obj66;
    lv_obj_t *settings4_item__obj67;
    lv_obj_t *settings4_item__obj68;
    lv_obj_t *settings4_item__obj69;
    lv_obj_t *settings4_item__obj70;
    lv_obj_t *front_up;
    lv_obj_t *front_down;
    lv_obj_t *back_up;
    lv_obj_t *back_down;
    lv_obj_t *ride;
    lv_obj_t *park;
    lv_obj_t *settings;
    lv_obj_t *back_pressure;
    lv_obj_t *back_pressure_label;
    lv_obj_t *front_pressure;
    lv_obj_t *front_pressure_label;
    lv_obj_t *obj0;
    lv_obj_t *obj1;
    lv_obj_t *obj2;
    lv_obj_t *obj3;
    lv_obj_t *obj4;
    lv_obj_t *obj5;
    lv_obj_t *obj6;
    lv_obj_t *obj7;
    lv_obj_t *obj8;
    lv_obj_t *return_to_update_select;
    lv_obj_t *return_to_update_select_label;
    lv_obj_t *update_screen_item_name;
    lv_obj_t *update_screen_item_percentage;
    lv_obj_t *update_screen_item_status;
    lv_obj_t *update_failed_button;
    lv_obj_t *obj9;
    lv_obj_t *update_to_home;
    lv_obj_t *return_to_settings_label;
    lv_obj_t *previous_item;
    lv_obj_t *previous_item_label;
    lv_obj_t *next_item;
    lv_obj_t *next_item_label;
    lv_obj_t *update_item_container;
    lv_obj_t *update_select_loading;
    lv_obj_t *top_left_button;
    lv_obj_t *bottom_right_button;
    lv_obj_t *calibration_text_label;
    lv_obj_t *obj10;
    lv_obj_t *obj11;
    lv_obj_t *obj12;
    lv_obj_t *obj13;
    lv_obj_t *obj14;
    lv_obj_t *obj15;
    lv_obj_t *obj16;
    lv_obj_t *settings_item_container;
    lv_obj_t *obj17;
    lv_obj_t *obj18;
    lv_obj_t *obj19;
} objects_t;

extern objects_t objects;

void create_screen_main_screen();
void delete_screen_main_screen();
void tick_screen_main_screen();

void create_screen_update_screen();
void delete_screen_update_screen();
void tick_screen_update_screen();

void create_screen_update_select_screen();
void delete_screen_update_select_screen();
void tick_screen_update_select_screen();

void create_screen_calibration_screen();
void delete_screen_calibration_screen();
void tick_screen_calibration_screen();

void create_screen_settings_select_screen();
void delete_screen_settings_select_screen();
void tick_screen_settings_select_screen();

void create_user_widget_update_item(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_update_item(void *flowState, int startWidgetIndex);

void create_user_widget_settings1(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_settings1(void *flowState, int startWidgetIndex);

void create_user_widget_settings2(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_settings2(void *flowState, int startWidgetIndex);

void create_user_widget_settings3(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_settings3(void *flowState, int startWidgetIndex);

void create_user_widget_settings4(lv_obj_t *parent_obj, void *flowState, int startWidgetIndex);
void tick_user_widget_settings4(void *flowState, int startWidgetIndex);

void create_screen_by_id(enum ScreensEnum screenId);
void delete_screen_by_id(enum ScreensEnum screenId);
void tick_screen_by_id(enum ScreensEnum screenId);
void tick_screen(int screen_index);

void create_screens();

#ifdef __cplusplus
}
#endif

#endif /*EEZ_LVGL_UI_SCREENS_H*/