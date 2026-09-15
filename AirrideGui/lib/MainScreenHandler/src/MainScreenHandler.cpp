#include "MainScreenHandler.h"

#include <stdio.h>

#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "vars.h"

/** Room for "%.1f" of a reading, plus a sign and a terminator. */
static constexpr size_t BAR_TEXT_WIDTH = 12;

MainScreenHandler *MainScreenHandler::_active = nullptr;

/**
 * @brief Bind the readings, the bus and the settings, and become Active.
 */
MainScreenHandler::MainScreenHandler(MainScreenData &data, MainScreenCommunication &communication,
                                     SettingsDevice &settings)
    : _data(data), _communication(communication), _settings(settings) {
    _active = this;
}

/**
 * @brief Cancel a pending auto ride and deregister.
 */
MainScreenHandler::~MainScreenHandler() {
    CancelAutoRide();
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the var and action shims talk to.
 * @return The Active handler, or null before one exists.
 */
MainScreenHandler *MainScreenHandler::Active() {
    return _active;
}

/**
 * @brief Start listening as the main screen comes up.
 * @param e Unused.
 * @note On load start rather than on loaded, so the readings are already
 *       arriving by the time the screen finishes fading in.
 */
void MainScreenHandler::OnMainScreenLoadStart(lv_event_t *e) {
    (void) e;
    _communication.Subscribe();
}

/**
 * @brief Stop listening as the main screen goes away.
 * @param e Unused.
 * @note The pending automatic lift goes with it - it would otherwise fire
 *       against readings that stopped arriving the moment this ran. Coming
 *       back to the screen arms it again.
 */
void MainScreenHandler::OnMainScreenUnloadStart(lv_event_t *e) {
    (void) e;
    StopAutoRideTimer();
    _communication.Unsubscribe();
}

/**
 * @brief Push the settings to the controller and arm the auto ride.
 * @param e Unused.
 * @note Runs on every entry, so a Save on the settings screen reaches the
 *       controller on the way back without anything else noticing.
 */
void MainScreenHandler::OnMainScreenLoaded(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Main screen loaded, front/back", _data.front, _data.back);
    _communication.SendSettings();
    ArmAutoRideTimer();
}

/**
 * @brief Front up pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontUpPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front up pressed");
    SendButton(FRONT_UP, true);
}

/**
 * @brief Front up released.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontUpReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front up released");
    SendButton(FRONT_UP, false);
}

/**
 * @brief Front down pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontDownPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front down pressed");
    SendButton(FRONT_DOWN, true);
}

/**
 * @brief Front down released.
 * @param e Unused.
 */
void MainScreenHandler::OnFrontDownReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Front down released");
    SendButton(FRONT_DOWN, false);
}

/**
 * @brief Back up pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnBackUpPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back up pressed");
    SendButton(BACK_UP, true);
}

/**
 * @brief Back up released.
 * @param e Unused.
 */
void MainScreenHandler::OnBackUpReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back up released");
    SendButton(BACK_UP, false);
}

/**
 * @brief Back down pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnBackDownPressed(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back down pressed");
    SendButton(BACK_DOWN, true);
}

/**
 * @brief Back down released.
 * @param e Unused.
 */
void MainScreenHandler::OnBackDownReleased(lv_event_t *e) {
    (void) e;
    LOG_DEBUG("Back down released");
    SendButton(BACK_DOWN, false);
}

/**
 * @brief Park pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnParkPressed(lv_event_t *e) {
    (void) e;
    LOG_INFO("Park pressed");
    SendButton(PARK, true);
}

/**
 * @brief Ride pressed.
 * @param e Unused.
 */
void MainScreenHandler::OnRidePressed(lv_event_t *e) {
    (void) e;
    LOG_INFO("Ride pressed");
    SendButton(RIDE, true);
}

/**
 * @brief Send one control frame and cancel the automatic lift.
 * @param button Which control was used.
 * @param state  true while held, false on release.
 */
void MainScreenHandler::SendButton(EMainScreenButtons button, bool state) {
    CancelAutoRide();
    _communication.SendMessageButtonPress(button, state);
}

/**
 * @brief Arm the one-shot automatic lift, if the setting asks for it.
 * @note Re-entering the main screen does not re-arm it; the lift is meant
 *       to happen once, shortly after the car is switched on.
 */
void MainScreenHandler::ArmAutoRideTimer() {
    if (!_settings.autoRide) {
        LOG_DEBUG("Auto start ride is disabled");
        return;
    }
    if (_abortAutoRide) {
        LOG_DEBUG("Auto start ride already run or cancelled");
        return;
    }
    if (_autoRideTimer != nullptr) {
        LOG_DEBUG("Auto ride timer already armed");
        return;
    }

    const uint32_t delayMs = (uint32_t) (_settings.autoRideSec * 1000.0f);
    _autoRideTimer = lv_timer_create(AutoRideTimerCb, delayMs, this);
    if (_autoRideTimer == nullptr) {
        LOG_ERROR("Could not create the auto ride timer");
        return;
    }
    lv_timer_set_repeat_count(_autoRideTimer, 1);
    LOG_DEBUG("Auto ride timer armed for ms", delayMs);
}

/**
 * @brief Drop a pending automatic lift and stop a later one being armed.
 */
void MainScreenHandler::CancelAutoRide() {
    _abortAutoRide = true;
    StopAutoRideTimer();
}

/**
 * @brief Drop a pending automatic lift, leaving a later one free to arm.
 * @note What leaving the screen wants. CancelAutoRide() is the user saying
 *       no, and that sticks for the rest of the run.
 */
void MainScreenHandler::StopAutoRideTimer() {
    if (_autoRideTimer != nullptr) {
        lv_timer_delete(_autoRideTimer);
        _autoRideTimer = nullptr;
    }
}

/**
 * @brief Timer callback for the automatic lift.
 * @param timer Carries the handler.
 */
void MainScreenHandler::AutoRideTimerCb(lv_timer_t *timer) {
    MainScreenHandler *self = (MainScreenHandler *) lv_timer_get_user_data(timer);
    self->_autoRideTimer = nullptr;
    self->AutoStartRide();
}

/**
 * @brief Lift the car unless it is already up, or the user intervened.
 */
void MainScreenHandler::AutoStartRide() {
    if (_abortAutoRide) {
        return;
    }
    _abortAutoRide = true;

    if (_data.front < AUTO_RIDE_PRESSURE_THRESHOLD || _data.back < AUTO_RIDE_PRESSURE_THRESHOLD) {
        LOG_INFO("Auto ride: car is down, sending ride command");
        _communication.SendMessageButtonPress(RIDE, true);
    } else {
        LOG_INFO("Auto ride: car is already up, nothing to do");
    }
}

/**
 * @brief Render one pressure the way the main screen labels want it.
 * @param buffer Per-variable store, kept alive for LVGL to copy from.
 * @param value The reading to render.
 * @return The buffer.
 * @note The labels take text, so the tenths formatting the flow engine used
 *       to do lives here now.
 */
static const char *FormatBar(char *buffer, size_t size, float value) {
    snprintf(buffer, size, "%.1f", value);
    return buffer;
}

/**
 * @brief Read front pressure as the label text.
 * @return The reading, to one decimal.
 */
extern "C" const char *get_var_front_pressure_text() {
    static char buffer[BAR_TEXT_WIDTH];
    MainScreenHandler *handler = MainScreenHandler::Active();
    return FormatBar(buffer, sizeof(buffer), handler != nullptr ? handler->FrontPressure() : 0.0f);
}

/**
 * @brief Read back pressure as the label text.
 * @return The reading, to one decimal.
 */
extern "C" const char *get_var_back_pressure_text() {
    static char buffer[BAR_TEXT_WIDTH];
    MainScreenHandler *handler = MainScreenHandler::Active();
    return FormatBar(buffer, sizeof(buffer), handler != nullptr ? handler->BackPressure() : 0.0f);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_main_screen_load_start(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnMainScreenLoadStart(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_main_screen_unload_start(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnMainScreenUnloadStart(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_main_screen_loaded(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnMainScreenLoaded(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_up_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontUpPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_up_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontUpReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_down_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontDownPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_front_down_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnFrontDownReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_up_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackUpPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_up_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackUpReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_down_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackDownPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_back_down_released(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnBackDownReleased(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_park_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnParkPressed(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_ride_pressed(lv_event_t *e) {
    MainScreenHandler *handler = MainScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnRidePressed(e);
    }
}
