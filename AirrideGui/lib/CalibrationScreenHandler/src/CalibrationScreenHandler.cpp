#include "CalibrationScreenHandler.h"
#include <stdio.h>
#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "ui.h"

CalibrationScreenHandler *CalibrationScreenHandler::_active = nullptr;

/**
 * @brief Bind the settings, their storage and the touch controller.
 */
CalibrationScreenHandler::CalibrationScreenHandler(SettingsDevice &settings, SettingsStorage &storage,
                                                   XPT2046_Bitbang &touch)
    : _settings(settings), _storage(storage), _touch(touch) {
    if (_active != nullptr) {
        LOG_WARN("Second CalibrationScreenHandler built, replacing the Active one");
    }
    _active = this;
}

/**
 * @brief Drop any running timer and deregister as the Active handler.
 */
CalibrationScreenHandler::~CalibrationScreenHandler() {
    if (_timer != nullptr) {
        lv_timer_delete(_timer);
        _timer = nullptr;
    }
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the action shims talk to.
 * @return The Active handler, or null before one exists.
 */
CalibrationScreenHandler *CalibrationScreenHandler::Active() {
    return _active;
}

/**
 * @brief Apply the stored mapping to the touch controller.
 */
void CalibrationScreenHandler::ApplyStoredCalibration() {
    if (!_settings.calibrationSet) {
        LOG_INFO("No stored calibration, keeping the built-in defaults");
        return;
    }

    LOG_DEBUG("Applying stored calibration:", _settings.xmin, _settings.xmax, _settings.ymin, _settings.ymax);
    _touch.setCalibration(_settings.xmin, _settings.xmax, _settings.ymin, _settings.ymax);
}

/**
 * @brief Start the sequence when the screen is loaded.
 * @param e Unused.
 */
void CalibrationScreenHandler::OnCalibrationScreenLoadStarted(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Calibration screen loading, restarting the sequence");
    EnterStartCountdown();
}

/**
 * @brief Tidy up on the way out.
 * @param e Unused.
 */
void CalibrationScreenHandler::OnLeaveCalibrationScreen(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Leaving calibration screen");
    SetState(ECalibrationState::Idle);
    StopTimer();
}

/**
 * @brief Top-left target was pressed.
 * @param e Unused.
 */
void CalibrationScreenHandler::OnTopLeftButtonPressed(lv_event_t *e) {
    (void) e;
    OnCornerEvent(ECalibrationState::TouchTopLeft);
}

/**
 * @brief Bottom-right target was pressed.
 * @param e Unused.
 */
void CalibrationScreenHandler::OnBottomRightButtonPressed(lv_event_t *e) {
    (void) e;
    OnCornerEvent(ECalibrationState::TouchBottomRight);
}

/**
 * @brief Handle a press on whichever corner is being sampled.
 * @param corner The corner the press belongs to.
 */
void CalibrationScreenHandler::OnCornerEvent(ECalibrationState corner) {
    if (_state != corner) {
        return;
    }
    if (!_windowOpen) {
        _windowOpen = true;
        _sumX = 0;
        _sumY = 0;
        _sampleCount = 0;
        StartTimer(MEASURE_MS);
        SetText("Hold still...");
    }

    AddSample();
}

/**
 * @brief Count down before the first corner is asked for.
 */
void CalibrationScreenHandler::EnterStartCountdown() {
    _xmin = 0;
    _ymin = 0;
    _xmax = RAW_MAX;
    _ymax = RAW_MAX;

    ShowCorner(objects.top_left_button, false);
    ShowCorner(objects.bottom_right_button, false);

    _windowOpen = false;
    SetState(ECalibrationState::StartCountdown);
    _secondsLeft = COUNTDOWN_SECONDS;
    StartTimer(TICK_MS);
    ShowCountdown("Starting calibration in:");
}

/**
 * @brief Ask for the top-left corner.
 */
void CalibrationScreenHandler::EnterTouchTopLeft() {
    ShowCorner(objects.bottom_right_button, false);
    ShowCorner(objects.top_left_button, true);
    SetText("Press and hold\nthe top left corner");
    SetState(ECalibrationState::TouchTopLeft);
    _windowOpen = false;
    StopTimer();
}

/**
 * @brief Ask for the bottom-right corner.
 */
void CalibrationScreenHandler::EnterTouchBottomRight() {
    ShowCorner(objects.top_left_button, false);
    ShowCorner(objects.bottom_right_button, true);
    SetText("Press and hold\nthe bottom right corner");
    SetState(ECalibrationState::TouchBottomRight);
    _windowOpen = false;
    StopTimer();
}

/**
 * @brief Take one raw reading for the corner being sampled.
 */
void CalibrationScreenHandler::AddSample() {
    int x = 0;
    int y = 0;
    if (!ReadRaw(x, y)) {
        return;
    }

    _sumX += (uint32_t) x;
    _sumY += (uint32_t) y;
    _sampleCount++;
}

/**
 * @brief Close the sampling window and reduce it to one point.
 */
void CalibrationScreenHandler::FinishWindow() {
    _windowOpen = false;
    const int x = (int) (_sumX / _sampleCount);
    const int y = (int) (_sumY / _sampleCount);

    if (_state == ECalibrationState::TouchTopLeft) {
        _xmin = x;
        _ymin = y;
        LOG_DEBUG("Xmin:", _xmin, "Ymin:", _ymin, "from", _sampleCount, "readings");
    } else {
        _xmax = x;
        _ymax = y;
        LOG_DEBUG("Xmax:", _xmax, "Ymax:", _ymax, "from", _sampleCount, "readings");
    }
}

/**
 * @brief Store the mapping, apply it, and count down to leaving.
 */
void CalibrationScreenHandler::SaveAndEnterExitCountdown() {
    ShowCorner(objects.top_left_button, false);
    ShowCorner(objects.bottom_right_button, false);

    const int xMargin = (RAW_MAX / SCREEN_W) * CROSS_HALF_PX;
    const int yMargin = (RAW_MAX / SCREEN_H) * CROSS_HALF_PX;

    _xmin -= xMargin;
    _xmax += xMargin;
    _ymin -= yMargin;
    _ymax += yMargin;

    if (_xmin < 0) _xmin = 0;
    if (_ymin < 0) _ymin = 0;
    if (_xmax > RAW_MAX) _xmax = RAW_MAX;
    if (_ymax > RAW_MAX) _ymax = RAW_MAX;

    LOG_DEBUG("Saving calibration values:", _xmin, _xmax, _ymin, _ymax);

    _touch.setCalibration(_xmin, _xmax, _ymin, _ymax);

    _settings.xmin = _xmin;
    _settings.xmax = _xmax;
    _settings.ymin = _ymin;
    _settings.ymax = _ymax;
    _settings.calibrationSet = true;

    _storage.WriteSettingsAsync(_settings);

    SetState(ECalibrationState::ExitCountdown);
    _secondsLeft = COUNTDOWN_SECONDS;
    StartTimer(TICK_MS);
    ShowCountdown("Exit calibration in:");
}

/**
 * @brief Switch to the main screen.
 */
void CalibrationScreenHandler::LeaveToMainScreen() {
    SetState(ECalibrationState::Idle);
    StopTimer();
    loadScreenNoAnim(SCREEN_ID_MAIN_SCREEN);
}

/**
 * @brief Drive the state machine. Called once a second by the timer.
 */
void CalibrationScreenHandler::Tick() {
    if (lv_screen_active() != objects.calibration_screen) {
        SetState(ECalibrationState::Idle);
        StopTimer();
        return;
    }

    switch (_state) {
        case ECalibrationState::StartCountdown:
            if (_secondsLeft <= 0) {
                EnterTouchTopLeft();
            } else {
                _secondsLeft--;
                StartTimer(TICK_MS);
                ShowCountdown("Starting calibration in:");
            }
            break;

        case ECalibrationState::TouchTopLeft:
            FinishWindow();
            EnterTouchBottomRight();
            break;
        case ECalibrationState::TouchBottomRight:
            FinishWindow();
            SaveAndEnterExitCountdown();
            break;

        case ECalibrationState::ExitCountdown:
            if (_secondsLeft <= 0) {
                LeaveToMainScreen();
            } else {
                _secondsLeft--;
                StartTimer(TICK_MS);
                ShowCountdown("Exit calibration in:");
            }
            break;

        case ECalibrationState::Idle:
            StopTimer();
            break;
    }
}

/**
 * @brief Move to a state.
 * @param state The state to enter.
 */
void CalibrationScreenHandler::SetState(ECalibrationState state) {
    _state = state;
}

/**
 * @brief Put a caption and the remaining seconds on screen.
 * @param caption Text before the count.
 */
void CalibrationScreenHandler::ShowCountdown(const char *caption) const {
    char text[64];
    snprintf(text, sizeof(text), "%s\n%d", caption, _secondsLeft);
    SetText(text);
}

/**
 * @brief Read one raw touch point.
 * @param x Receives the raw x.
 * @param y Receives the raw y.
 * @return false when nothing is being touched.
 */
bool CalibrationScreenHandler::ReadRaw(int &x, int &y) const {
    const TouchPoint touch = _touch.getTouch();
    if (touch.zRaw == 0) {
        return false;
    }

    x = touch.xRaw;
    y = touch.yRaw;
    return true;
}

/**
 * @brief Show or hide one corner target.
 * @param button The target.
 * @param shown Whether it should be visible.
 */
void CalibrationScreenHandler::ShowCorner(lv_obj_t *button, bool shown) const {
    if (button == nullptr) {
        return;
    }

    if (shown) {
        lv_obj_remove_flag(button, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(button, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Set the instruction label.
 * @param text What to show.
 */
void CalibrationScreenHandler::SetText(const char *text) const {
    if (objects.calibration_text_label == nullptr) {
        return;
    }

    lv_label_set_text(objects.calibration_text_label, text);
}

/**
 * @brief Start the one-second countdown timer.
 * @param seconds How long to count.
 */
void CalibrationScreenHandler::StartTimer(int seconds) {
    _timer = lv_timer_create(TimerCb, seconds, this);
    lv_timer_set_repeat_count(_timer, 1);
}

/**
 * @brief Stop the countdown timer.
 */
void CalibrationScreenHandler::StopTimer() {
    if (_timer != nullptr) {
        lv_timer_pause(_timer);
    }
}

/**
 * @brief Timer callback; forwards to Tick().
 * @param timer Carries the handler.
 */
void CalibrationScreenHandler::TimerCb(lv_timer_t *timer) {
    CalibrationScreenHandler *handler = (CalibrationScreenHandler *) lv_timer_get_user_data(timer);
    if (handler == nullptr) {
        return;
    }
    handler->Tick();
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_calibration_screen_load_started(lv_event_t *e) {
    CalibrationScreenHandler *handler = CalibrationScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Calibration screen loaded with no handler");
        return;
    }
    handler->OnCalibrationScreenLoadStarted(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_leave_calibration_screen(lv_event_t *e) {
    CalibrationScreenHandler *handler = CalibrationScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnLeaveCalibrationScreen(e);
    }
    loadScreen(SCREEN_ID_MAIN_SCREEN);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_top_left_button_pressed(lv_event_t *e) {
    CalibrationScreenHandler *handler = CalibrationScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Top left pressed with no handler");
        return;
    }
    handler->OnTopLeftButtonPressed(e);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_bottom_right_button_pressed(lv_event_t *e) {
    CalibrationScreenHandler *handler = CalibrationScreenHandler::Active();
    if (handler == nullptr) {
        LOG_WARN("Bottom right pressed with no handler");
        return;
    }
    handler->OnBottomRightButtonPressed(e);
}
