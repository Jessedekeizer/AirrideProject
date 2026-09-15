#include "ui.h"
#include "screens.h"
#include "images.h"
#include "actions.h"
#include "vars.h"

static int16_t currentScreen = -1;

static lv_obj_t *getLvglObjectFromIndex(int32_t index) {
    if (index == -1) {
        return 0;
    }
    return ((lv_obj_t **)&objects)[index];
}

static void setScreen(enum ScreensEnum screenId, lv_scr_load_anim_t anim, uint32_t speed) {
    currentScreen = screenId - 1;
    lv_obj_t *screen = getLvglObjectFromIndex(currentScreen);
    lv_scr_load_anim(screen, anim, speed, 0, false);
}

void loadScreen(enum ScreensEnum screenId) {
    setScreen(screenId, LV_SCR_LOAD_ANIM_FADE_IN, 200);
}

// For jumps the code forces rather than the user asking for, where the fade
// would only get in the way.
void loadScreenNoAnim(enum ScreensEnum screenId) {
    setScreen(screenId, LV_SCR_LOAD_ANIM_NONE, 0);
}

void ui_init() {
    create_screens();
    loadScreen(SCREEN_ID_MAIN_SCREEN);

}

void ui_tick() {
    tick_screen(currentScreen);
}