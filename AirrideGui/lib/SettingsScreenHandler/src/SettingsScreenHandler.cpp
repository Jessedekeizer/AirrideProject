#include "SettingsScreenHandler.h"

#include <stdint.h>
#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "ui.h"
#include "vars.h"

SettingsScreenHandler *SettingsScreenHandler::_active = nullptr;

extern "C" void (*tick_screen_funcs[])();

void (*SettingsScreenHandler::_generatedSettingsTick)() = nullptr;
bool SettingsScreenHandler::_settingsDirty = true;
uint32_t SettingsScreenHandler::_lastFullTickMs = 0;

/**
 * @brief Note that something a settings screen shows has changed.
 */
void SettingsScreenHandler::MarkDirty() {
    _settingsDirty = true;
}

/**
 * @brief Stands in for the generated settings tick.
 * @note The generated one re-evaluates 55 bindings and strcmps each result
 *       against the label. This runs it only when something changed, or
 *       when the backstop is due.
 */
void SettingsScreenHandler::SettingsScreenTickCb() {
    if (_generatedSettingsTick == nullptr) {
        return;
    }

    const uint32_t now = lv_tick_get();
    const bool refreshDue = (now - _lastFullTickMs) >= TICK_REFRESH_MS;

    if (!_settingsDirty && !refreshDue) {
        return;
    }

    _settingsDirty = false;
    _lastFullTickMs = now;
    _generatedSettingsTick();
}

/**
 * @brief Install the tick filter, keeping the generated one to call.
 */
void SettingsScreenHandler::InstallTickFilter() {
    if (_generatedSettingsTick != nullptr) {
        return;
    }

    const int index = SCREEN_ID_SETTINGS_SELECT_SCREEN - 1;
    _generatedSettingsTick = tick_screen_funcs[index];
    tick_screen_funcs[index] = SettingsScreenTickCb;
}

/**
 * @brief Bind the settings and their storage, and become Active.
 */
SettingsScreenHandler::SettingsScreenHandler(SettingsDevice &settings, SettingsStorage &storage)
    : _settings(settings), _storage(storage) {
    if (_active != nullptr) {
        LOG_WARN("Second SettingsScreenHandler built, replacing the Active one");
    }
    _active = this;
}

/**
 * @brief Drop a pending page refresh and deregister.
 */
SettingsScreenHandler::~SettingsScreenHandler() {
    CancelPageRefresh();
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the var and action shims talk to.
 * @return The Active handler, or null before one exists.
 */
SettingsScreenHandler *SettingsScreenHandler::Active() {
    return _active;
}

/**
 * @brief Write the settings to the card.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSaveSettingsPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Save settings pressed");
    Save();
}

/**
 * @brief The page container for an index.
 * @param index Page index.
 * @return The container, or null when out of range.
 */
lv_obj_t *SettingsScreenHandler::PageAt(int index) {
    switch (index) {
        case 0:
            return objects.settings1_item;
        case 1:
            return objects.settings2_item;
        case 2:
            return objects.settings3_item;
        case 3:
            return objects.settings4_item;
        default:
            return nullptr;
    }
}

/**
 * @brief The container the four pages sit in, side by side.
 * @return The container.
 */
lv_obj_t *SettingsScreenHandler::PagerContainer() {
    return objects.settings_item_container;
}

/**
 * @brief Width of one page, which is also the scroll step between them.
 * @return The stride in pixels, or 0 if nothing can be measured.
 */
int32_t SettingsScreenHandler::PageStride() {
    lv_obj_t *cont = PagerContainer();
    if (cont != nullptr) {
        lv_obj_update_layout(cont);
    }

    lv_obj_t *page = PageAt(0);
    if (page != nullptr) {
        const int32_t w = lv_obj_get_width(page);
        if (w > 0) {
            return w;
        }
    }

    if (cont != nullptr) {
        const int32_t w = lv_obj_get_width(cont);
        if (w > 0) {
            return w;
        }
    }

    lv_display_t *disp = lv_display_get_default();
    return disp != nullptr ? lv_display_get_horizontal_resolution(disp) : 0;
}

/**
 * @brief Schedule a scroll to the current page.
 * @warning The scroll is deferred rather than done here. Every caller is
 *          inside an LVGL event and lv_obj_scroll_to_x() dispatches events
 *          of its own, which crashed LVGL three levels deep.
 */
void SettingsScreenHandler::RefreshPage() {
    InstallTickFilter();

    if (_pageRefreshTimer != nullptr) {
        return;
    }
    _pageRefreshTimer = lv_timer_create(PageRefreshCb, 0, this);
    lv_timer_set_repeat_count(_pageRefreshTimer, 1);
}

/**
 * @brief Drop a pending page refresh.
 */
void SettingsScreenHandler::CancelPageRefresh() {
    if (_pageRefreshTimer != nullptr) {
        lv_timer_delete(_pageRefreshTimer);
        _pageRefreshTimer = nullptr;
    }
}

/**
 * @brief Timer callback that performs the deferred scroll.
 * @param timer Carries the handler.
 */
void SettingsScreenHandler::PageRefreshCb(lv_timer_t *timer) {
    SettingsScreenHandler *self = (SettingsScreenHandler *) lv_timer_get_user_data(timer);
    self->_pageRefreshTimer = nullptr;
    self->ApplyPage();
}

/**
 * @brief Scroll the pager to the current page, without animating.
 */
void SettingsScreenHandler::ApplyPage() {
    lv_obj_t *cont = PagerContainer();
    const int32_t stride = PageStride();
    if (cont == nullptr || stride <= 0) {
        LOG_WARN("Cannot page: container", cont != nullptr, "stride", stride);
        return;
    }

    lv_obj_scroll_to_x(cont, _pageIndex * stride, LV_ANIM_OFF);
}

/**
 * @brief Back one settings page.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSettingsLeftPressed(lv_event_t *e) {
    (void) e;
    if (_pageIndex <= 0) {
        LOG_DEBUG("Already at settings1, cannot go back");
        return;
    }
    _pageIndex--;
    LOG_DEBUG("Back to settings", _pageIndex + 1);
    RefreshPage();
}

/**
 * @brief Forward one settings page.
 * @param e Unused.
 */
void SettingsScreenHandler::OnSettingsRightPressed(lv_event_t *e) {
    (void) e;
    if (_pageIndex + 1 >= PAGE_COUNT) {
        LOG_DEBUG("Already at settings", PAGE_COUNT, "cannot go on");
        return;
    }
    _pageIndex++;
    LOG_DEBUG("Forward to settings", _pageIndex + 1);
    RefreshPage();
}

/**
 * @brief Reload from the card and start at page one.
 * @param e Unused.
 * @note Hooked to the screen load rather than the button, so it runs
 *       however the screen is reached and nothing stale is drawn.
 */
void SettingsScreenHandler::OnSettingsSelectLoadStart(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings screen loaded, reloading from card");
    Load();

    InstallTickFilter();
    _pageIndex = 0;
    RefreshPage();
}

/**
 * @brief Read the card into the settings.
 */
void SettingsScreenHandler::Load() {
    _storage.ReadSettings(_settings);
    MarkDirty();
}

/**
 * @brief Write the settings to the card, off this thread.
 */
void SettingsScreenHandler::Save() {
    _storage.WriteSettingsAsync(_settings);
}

/**
 * @brief The settings every page reads and writes.
 * @return The shared struct, or null before a handler exists.
 * @note The per-page shims in Settings1Vars.cpp through Settings4Vars.cpp all
 *       come through here, so they share one struct rather than a copy each.
 */
SettingsDevice *ActiveSettings() {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings var touched with no handler");
        return nullptr;
    }
    return &handler->Settings();
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 * @note The screen change used to hang off the flow chart after this action,
 *       and it ran whether or not the action did anything, so it still does.
 */
extern "C" void action_save_settings_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Save settings pressed with no handler");
    } else {
        handler->OnSaveSettingsPressed(e);
    }
    loadScreen(SCREEN_ID_MAIN_SCREEN);
}

/**
 * @brief Leave the settings screens for the update screens.
 * @param e Unused.
 * @note The update screen sets itself up on load, so this only navigates.
 */
extern "C" void action_settings_to_update_pressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings to update pressed");
    loadScreen(SCREEN_ID_UPDATE_SELECT_SCREEN);
}

/**
 * @brief Enter the settings screens from the main screen.
 * @param e Unused.
 * @note OnSettingsSelectLoadStart reloads from the card once the screen is
 *       up, so this only navigates.
 */
extern "C" void action_main_screen_to_settings_select_pressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Main screen to settings pressed");
    loadScreen(SCREEN_ID_SETTINGS_SELECT_SCREEN);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_left_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings left pressed with no handler");
        return;
    }
    handler->OnSettingsLeftPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_right_pressed(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings right pressed with no handler");
        return;
    }
    handler->OnSettingsRightPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_settings_select_load_start(lv_event_t *e) {
    SettingsScreenHandler *handler = SettingsScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Settings screen loaded with no handler");
        return;
    }
    handler->OnSettingsSelectLoadStart(e);
}

/**
 * @brief Leave the settings screens for the main screen.
 * @param e Unused.
 * @note The flow chart changed screen here without calling anything, so this
 *       exists only to give the button a native action to name.
 */
extern "C" void action_settings_select_to_main_pressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings select to main pressed");
    loadScreen(SCREEN_ID_MAIN_SCREEN);
}

/**
 * @brief Leave the settings screens for the calibration screen.
 * @param e Unused.
 * @note As above - the flow chart changed screen here on its own.
 */
extern "C" void action_settings_to_calibration_pressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Settings to calibration pressed");
    loadScreen(SCREEN_ID_CALIBRATION_SCREEN);
}
