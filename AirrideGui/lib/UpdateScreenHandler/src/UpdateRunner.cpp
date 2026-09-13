#include "UpdateRunner.h"

#include <stdint.h>

#include "Logger.h"
#include "screens.h"
#include "ui.h"

/**
 * @brief Bind the bus and the list whose selection this clears when done.
 */
UpdateRunner::UpdateRunner(OTACommunication &ota, UpdateDeviceList &list)
    : _ota(ota), _list(list) {
}

/**
 * @brief Drop every timer.
 */
UpdateRunner::~UpdateRunner() {
    Stop();
}

/**
 * @brief Arm the update; START is sent from StartTimerCb.
 * @param nodeId The node to update.
 * @param name   What to call it on the update screen.
 * @warning START is not sent here. This runs inside the button press and
 *          the screen changes only once it returns, so bringing up the WiFi
 *          access point here would block with the old screen still up.
 */
void UpdateRunner::Begin(int nodeId, const char *name) {
    _nodeId = nodeId;

    ShowSelected(name);

    _lastStatus = {};
    _lastStatus.type = EOTAStatusType::UPDATE;
    _lastStatusMs = lv_tick_get();
    _updating = true;

    if (_updateTimer == nullptr) {
        _updateTimer = lv_timer_create(UpdatePumpCb, UPDATE_PUMP_MS, this);
    }

    if (_startTimer != nullptr) {
        lv_timer_delete(_startTimer);
    }
    _startTimer = lv_timer_create(StartTimerCb, START_DELAY_MS, this);
    lv_timer_set_repeat_count(_startTimer, 1);
}

/**
 * @brief Send START once the update screen has been painted.
 * @param timer Carries the runner.
 */
void UpdateRunner::StartTimerCb(lv_timer_t *timer) {
    UpdateRunner *self = (UpdateRunner *) lv_timer_get_user_data(timer);
    self->_startTimer = nullptr;

    if (!self->_updating) {
        LOG_DEBUG("Update cancelled before it started");
        return;
    }

    LOG_INFO("Starting update on node", self->_nodeId);
    self->_lastStatusMs = lv_tick_get();
    self->_ota.SendStart((ECanNode) self->_nodeId);
}

/**
 * @brief Stop whatever is running and drop the timers.
 * @note The stop is broadcast: a node that took the START but never
 *       reported is exactly the one that has to hear it.
 */
void UpdateRunner::Stop() {
    CancelDoneRescan();
    if (_startTimer != nullptr) {
        lv_timer_delete(_startTimer);
        _startTimer = nullptr;
    }
    if (_updateTimer != nullptr) {
        lv_timer_delete(_updateTimer);
        _updateTimer = nullptr;
    }

    if (!_updating) {
        return;
    }
    _updating = false;

    _ota.SendStop(ECanNode::NODE_BROADCAST);
}

/**
 * @brief Fail the update when nothing has been heard for UPDATE_TIMEOUT_MS.
 * @param timer Carries the runner.
 */
void UpdateRunner::UpdatePumpCb(lv_timer_t *timer) {
    UpdateRunner *self = (UpdateRunner *) lv_timer_get_user_data(timer);

    if (!self->_updating) {
        return;
    }
    if (lv_tick_elaps(self->_lastStatusMs) < UPDATE_TIMEOUT_MS) {
        return;
    }

    LOG_ERROR("No update status for", (int) UPDATE_TIMEOUT_MS, "ms, giving up");
    self->_updating = false;
    self->_lastStatus.phase = EOTAUpdatePhase::ERROR;
    self->ShowStatus();
}

/**
 * @brief Take one update status, from a remote node or the access point.
 * @param status Type, phase and progress.
 */
void UpdateRunner::OnStatus(const CANAirRideOTAStatus &status) {
    if (!_updating) {
        LOG_DEBUG("Update status with no update running, ignoring");
        return;
    }

    _lastStatus = status;
    _lastStatusMs = lv_tick_get();

    if (status.phase == EOTAUpdatePhase::COMPLETE || status.phase == EOTAUpdatePhase::ERROR) {
        _updating = false;
        if (_updateTimer != nullptr) {
            lv_timer_delete(_updateTimer);
            _updateTimer = nullptr;
        }
    }

    if (status.phase == EOTAUpdatePhase::COMPLETE && _doneTimer == nullptr) {
        _doneTimer = lv_timer_create(DoneRescanCb, DONE_RESCAN_MS, this);
        lv_timer_set_repeat_count(_doneTimer, 1);
    }

    ShowStatus();
}

/**
 * @brief Drop a pending automatic return to the device list.
 */
void UpdateRunner::CancelDoneRescan() {
    if (_doneTimer != nullptr) {
        lv_timer_delete(_doneTimer);
        _doneTimer = nullptr;
    }
}

/**
 * @brief Return to the device list once a finished update has been read.
 * @param timer Carries the runner.
 * @note The only place this library changes screen itself; everywhere else
 *       that is a button.
 */
void UpdateRunner::DoneRescanCb(lv_timer_t *timer) {
    UpdateRunner *self = (UpdateRunner *) lv_timer_get_user_data(timer);
    self->_doneTimer = nullptr;

    LOG_INFO("Update finished, back to the device list");
    self->_list.ClearSelection();

    loadScreenNoAnim(SCREEN_ID_UPDATE_SELECT_SCREEN);
}

/**
 * @brief Put the last status on screen: phase in words, progress on the bar.
 * @note Repaints immediately during a self update, because ArduinoOTA
 *       blocks the loop for the whole image and lv_timer_handler() would
 *       never get another turn. Only safe from inside the pump.
 */
void UpdateRunner::ShowStatus() {
    const char *phase = "IDLE";
    switch (_lastStatus.phase) {
        case EOTAUpdatePhase::AP_STARTED:
            phase = "AP UP";
            break;
        case EOTAUpdatePhase::FLASHING:
            phase = "FLASHING";
            break;
        case EOTAUpdatePhase::COMPLETE:
            phase = "DONE";
            break;
        case EOTAUpdatePhase::ERROR:
            phase = "ERROR";
            break;
        default:
            break;
    }

    SetStatusText(phase);
    SetProgress(_lastStatus.progress);
    ShowRetry(_lastStatus.phase == EOTAUpdatePhase::ERROR);

    if (_ota.IsPumping()) {
        lv_refr_now(NULL);
    }
}

/**
 * @brief Put the selected device on the update screen before it is shown.
 * @param name What to call it.
 */
void UpdateRunner::ShowSelected(const char *name) {
    lv_label_set_text(objects.update_screen_item_name, name);

    SetProgress(0);
    SetStatusText("IDLE");
    ShowRetry(false);
}

/**
 * @brief Swap the update screen between its two exits.
 * @param retry true after a failure: back is hidden and retry takes its
 *              place. Both run the same action and screen change.
 */
void UpdateRunner::ShowRetry(bool retry) {
    lv_obj_t *back = objects.return_to_update_select;
    lv_obj_t *again = objects.update_failed_button;

    if (retry) {
        lv_obj_add_flag(back, LV_OBJ_FLAG_HIDDEN);
        lv_obj_remove_flag(again, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_remove_flag(back, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(again, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Move the progress bar.
 * @param percent 0..100, clamped.
 * @warning Only ours to write while the bar Value stays literal in the EEZ
 *          project; an expression makes the tick overwrite it.
 */
void UpdateRunner::SetProgress(int percent) {
    if (percent < 0) {
        percent = 0;
    } else if (percent > 100) {
        percent = 100;
    }
    lv_bar_set_value(objects.update_screen_item_percentage, percent, LV_ANIM_OFF);
}

/**
 * @brief Set the status line on the update screen.
 * @param status Text to show.
 */
void UpdateRunner::SetStatusText(const char *status) {
    lv_label_set_text(objects.update_screen_item_status, status);
}
