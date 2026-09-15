#include "UpdateScreenHandler.h"

#include <stdint.h>

#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "ui.h"

UpdateScreenHandler *UpdateScreenHandler::_active = nullptr;

/**
 * @brief Bind the OTA communication and become the Active handler.
 */
UpdateScreenHandler::UpdateScreenHandler(OTACommunication &ota)
    : _ota(ota), _runner(ota, _list) {
    _active = this;
}

/**
 * @brief Drop a running scan and deregister as the Active handler.
 */
UpdateScreenHandler::~UpdateScreenHandler() {
    StopScanTimer();
    if (_active == this) {
        _active = nullptr;
    }
}

/**
 * @brief The handler the action shims talk to.
 * @return The Active handler, or null before one exists.
 */
UpdateScreenHandler *UpdateScreenHandler::Active() {
    return _active;
}

/**
 * @brief Route OTA status messages here.
 * @warning Call from setup(), not the constructor: static init order
 *          against Communication is not defined.
 */
void UpdateScreenHandler::Begin() {
    _ota.Init([this](ECanNode node, const CANAirRideOTAStatus &status) {
        OnOtaStatus(node, status);
    });
}

/**
 * @brief Hand one status to whichever half it belongs to.
 * @param node   Who reported.
 * @param status Type, phase and progress.
 */
void UpdateScreenHandler::OnOtaStatus(ECanNode node, const CANAirRideOTAStatus &status) {
    switch (status.type) {
        case EOTAStatusType::AVAILABLE:
            _list.Add(node);
            break;

        case EOTAStatusType::UPDATE:
            _runner.OnStatus(status);
            break;

        default:
            LOG_ERROR("Unknown OTA status type", (uint8_t) status.type);
            break;
    }
}

/**
 * @brief Empty the list and show the spinner before the screen paints.
 * @param e Unused.
 * @note Has to run here rather than on load: the fade in only ends once the
 *       screen is up, so anything the last scan left would be on show for the
 *       length of it.
 */
void UpdateScreenHandler::OnUpdateSelectScreenLoadStart(lv_event_t *e) {
    (void) e;

    _list.Prepare();
    _list.BeginScan();
}

/**
 * @brief Start a scan once the screen is up.
 * @param e Unused.
 */
void UpdateScreenHandler::OnUpdateSelectScreenLoaded(lv_event_t *e) {
    (void) e;
    LOG_INFO("Update select screen loaded, scanning for devices");

    // Covers the update screen too: it is only ever reached from here, and
    // the subscription is held until the exit button gives it back.
    _ota.Subscribe();

    _list.Prepare();

    StartScan();
}

/**
 * @brief Broadcast discovery and collect answers for SCAN_DURATION_MS.
 */
void UpdateScreenHandler::StartScan() {
    StopScanTimer();

    _list.BeginScan();

    _ota.SendDiscover();

    _scanTimer = lv_timer_create(ScanTimerCb, SCAN_DURATION_MS, this);
    lv_timer_set_repeat_count(_scanTimer, 1);
}

/**
 * @brief Drop a running discovery window.
 */
void UpdateScreenHandler::StopScanTimer() {
    if (_scanTimer != nullptr) {
        lv_timer_delete(_scanTimer);
        _scanTimer = nullptr;
    }
}

/**
 * @brief Timer callback closing the discovery window.
 * @param timer Carries the handler.
 */
void UpdateScreenHandler::ScanTimerCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);
    self->_scanTimer = nullptr;
    self->_list.EndScan();
}

/**
 * @brief Move to the next discovered device.
 * @param e Unused.
 */
void UpdateScreenHandler::OnNextUpdateItem(lv_event_t *e) {
    (void) e;
    _list.Next();
}

/**
 * @brief Move to the previous discovered device.
 * @param e Unused.
 */
void UpdateScreenHandler::OnPreviousUpdateItem(lv_event_t *e) {
    (void) e;
    _list.Previous();
}

/**
 * @brief Select the pressed device and arm its update.
 * @param e Carries the widget that was pressed.
 * @note The screen changes once this returns, so everything set here is in
 *       place before the update screen appears.
 */
void UpdateScreenHandler::OnUpdateItemPressed(lv_event_t *e) {
    if (!_list.SelectFromObject((lv_obj_t *) lv_event_get_target(e))) {
        return;
    }

    LOG_INFO("Update requested for", _list.SelectedName(), _list.SelectedId());
    _runner.Begin(_list.SelectedId(), _list.SelectedName());
}

/**
 * @brief Abandon whatever is running and go back to the list.
 * @param e Unused.
 */
void UpdateScreenHandler::OnReturnToUpdateSelect(lv_event_t *e) {
    (void) e;
    LOG_INFO("Leaving update screen, back to update select");

    _runner.Stop();
    _list.ClearSelection();
}

/**
 * @brief Drop a scan and any pending spinner on the way to the main screen.
 * @param e Unused.
 */
void UpdateScreenHandler::OnUpdateSelectToMain(lv_event_t *e) {
    (void) e;
    LOG_INFO("Leaving update select for the main screen");

    // The only way out of the two update screens, so the only place the
    // subscription has to be given back. A self update keeps it: SelfOTA is
    // mid flight and its status still has to land somewhere.
    if (!_ota.IsLocalActive()) {
        _ota.Unsubscribe();
    }

    StopScanTimer();
    _list.AbortScan();
    _runner.CancelDoneRescan();
    _list.ClearSelection();
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_update_select_screen_load_start(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnUpdateSelectScreenLoadStart(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_update_select_screen_loaded(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnUpdateSelectScreenLoaded(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_next_update_item(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnNextUpdateItem(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_previous_update_item(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnPreviousUpdateItem(e);
    }
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_update_item_update_pressed(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnUpdateItemPressed(e);
    }
    loadScreen(SCREEN_ID_UPDATE_SCREEN);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_update_to_update_select_pressed(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnReturnToUpdateSelect(e);
    }
    loadScreen(SCREEN_ID_UPDATE_SELECT_SCREEN);
}

/**
 * @brief Shim for the generated action of the same name.
 * @param e LVGL event.
 */
extern "C" void action_update_select_to_main_pressed(lv_event_t *e) {
    UpdateScreenHandler *handler = UpdateScreenHandler::Active();
    if (handler != nullptr) {
        handler->OnUpdateSelectToMain(e);
    }
    loadScreen(SCREEN_ID_MAIN_SCREEN);
}
