#ifndef SETTINGSSCREENHANDLER_H
#define SETTINGSSCREENHANDLER_H

#include <lvgl.h>

#include "SettingsDevice.h"
#include "SettingsStorage.h"

class SettingsScreenHandler {
public:
    SettingsScreenHandler(SettingsDevice &settings, SettingsStorage &storage);

    ~SettingsScreenHandler();

    static SettingsScreenHandler *Active();

    void OnSettingsSelectLoadStart(lv_event_t *e);

    void OnSaveSettingsPressed(lv_event_t *e);

    void OnSettingsLeftPressed(lv_event_t *e);

    void OnSettingsRightPressed(lv_event_t *e);

    static void MarkDirty();

    SettingsDevice &Settings() { return _settings; }

    void Load();

    void Save();

private:
    SettingsScreenHandler(const SettingsScreenHandler &) = delete;
    SettingsScreenHandler &operator=(const SettingsScreenHandler &) = delete;

    static constexpr int PAGE_COUNT = 4;

    void RefreshPage();

    static void PageRefreshCb(lv_timer_t *timer);

    void ApplyPage();

    void CancelPageRefresh();

    static lv_obj_t *PagerContainer();

    static lv_obj_t *PageAt(int index);

    static int32_t PageStride();

    static constexpr uint32_t TICK_REFRESH_MS = 250;

    static void SettingsScreenTickCb();

    static void InstallTickFilter();

    static void (*_generatedSettingsTick)();
    static bool _settingsDirty;
    static uint32_t _lastFullTickMs;

    static SettingsScreenHandler *_active;

    SettingsDevice &_settings;
    SettingsStorage &_storage;

    int _pageIndex = 0;

    lv_timer_t *_pageRefreshTimer = nullptr;
};

#endif
