#ifndef MAINSCREENHANDLER_H
#define MAINSCREENHANDLER_H

#include <lvgl.h>

#include "EMainScreenButtons.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "SettingsDevice.h"

class MainScreenHandler {
public:
    MainScreenHandler(MainScreenData &data, MainScreenCommunication &communication, SettingsDevice &settings);

    ~MainScreenHandler();

    static MainScreenHandler *Active();

    void OnMainScreenLoadStart(lv_event_t *e);

    void OnMainScreenLoaded(lv_event_t *e);

    void OnMainScreenUnloadStart(lv_event_t *e);

    void OnFrontUpPressed(lv_event_t *e);

    void OnFrontUpReleased(lv_event_t *e);

    void OnFrontDownPressed(lv_event_t *e);

    void OnFrontDownReleased(lv_event_t *e);

    void OnBackUpPressed(lv_event_t *e);

    void OnBackUpReleased(lv_event_t *e);

    void OnBackDownPressed(lv_event_t *e);

    void OnBackDownReleased(lv_event_t *e);

    void OnParkPressed(lv_event_t *e);

    void OnRidePressed(lv_event_t *e);

    float FrontPressure() const { return _data.front; }

    float BackPressure() const { return _data.back; }

private:
    MainScreenHandler(const MainScreenHandler &) = delete;
    MainScreenHandler &operator=(const MainScreenHandler &) = delete;

    static constexpr float AUTO_RIDE_PRESSURE_THRESHOLD = 1.5f;

    void SendButton(EMainScreenButtons button, bool state);

    void ArmAutoRideTimer();

    void CancelAutoRide();

    void StopAutoRideTimer();

    void AutoStartRide();

    static void AutoRideTimerCb(lv_timer_t *timer);

    static MainScreenHandler *_active;

    MainScreenData &_data;
    MainScreenCommunication &_communication;
    SettingsDevice &_settings;

    lv_timer_t *_autoRideTimer = nullptr;
    bool _abortAutoRide = false;
};

#endif
