#ifndef CALIBRATIONSCREENHANDLER_H
#define CALIBRATIONSCREENHANDLER_H

#include <lvgl.h>
#include <XPT2046_Bitbang.h>
#include "SettingsDevice.h"
#include "SettingsStorage.h"
#include "ECalibrationState.h"

class CalibrationScreenHandler {
public:

    CalibrationScreenHandler(SettingsDevice &settings, SettingsStorage &storage, XPT2046_Bitbang &touch);

    ~CalibrationScreenHandler();
    static CalibrationScreenHandler *Active();
    void OnCalibrationScreenLoadStarted(lv_event_t *e);
    void OnLeaveCalibrationScreen(lv_event_t *e);
    void OnTopLeftButtonPressed(lv_event_t *e);
    void OnBottomRightButtonPressed(lv_event_t *e);
    bool CalibrationRequired() const { return !_settings.calibrationSet; }
    void ApplyStoredCalibration();

private:

    CalibrationScreenHandler(const CalibrationScreenHandler &) = delete;
    CalibrationScreenHandler &operator=(const CalibrationScreenHandler &) = delete;

    static constexpr uint32_t TICK_MS = 1000;
    static constexpr int COUNTDOWN_SECONDS = 3;
    static constexpr int MEASURE_MS = 3000;
    static constexpr int RAW_MAX = 4095;
    static constexpr int CROSS_HALF_PX = 10;
    static constexpr int SCREEN_W = 320;
    static constexpr int SCREEN_H = 240;

    static void TimerCb(lv_timer_t *timer);
    void Tick();

    void EnterStartCountdown();
    void EnterTouchTopLeft();
    void EnterTouchBottomRight();
    void SaveAndEnterExitCountdown();
    void LeaveToMainScreen();

    void OnCornerEvent(ECalibrationState corner);
    void AddSample();
    void FinishWindow();
    void SetState(ECalibrationState state);

    void ShowCountdown(const char *caption) const;
    bool ReadRaw(int &x, int &y) const;

    void ShowCorner(lv_obj_t *button, bool shown) const;
    void SetText(const char *text) const;

    void StartTimer(int seconds);
    void StopTimer();

    static CalibrationScreenHandler *_active;

    SettingsDevice &_settings;
    SettingsStorage &_storage;
    XPT2046_Bitbang &_touch;

    lv_timer_t *_timer = nullptr;

    ECalibrationState _state = ECalibrationState::Idle;
    bool _windowOpen = false;

    int _secondsLeft = 0;
    uint32_t _sumX = 0;
    uint32_t _sumY = 0;
    uint32_t _sampleCount = 0;

    int _xmin = 0;
    int _xmax = RAW_MAX;
    int _ymin = 0;
    int _ymax = RAW_MAX;
};

#endif
