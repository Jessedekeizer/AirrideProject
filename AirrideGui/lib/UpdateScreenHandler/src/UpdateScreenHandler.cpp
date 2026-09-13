#include "UpdateScreenHandler.h"

#include <stdint.h>
#include <string.h>

#include "Logger.h"
#include "actions.h"
#include "screens.h"
#include "ui.h"

UpdateScreenHandler *UpdateScreenHandler::_active = nullptr;

/**
 * @brief Bind the OTA communication and become the Active handler.
 */
UpdateScreenHandler::UpdateScreenHandler(OTACommunication &ota)
    : _ota(ota) {
    _active = this;
}

/**
 * @brief Deregister as the Active handler.
 */
UpdateScreenHandler::~UpdateScreenHandler() {
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
 * @brief Empty the list, show the spinner and start a scan.
 * @param e Unused.
 */
void UpdateScreenHandler::OnUpdateSelectScreenLoaded(lv_event_t *e) {
    (void) e;
    LOG_INFO("Update select screen loaded, scanning for devices");

    // Covers the update screen too: it is only ever reached from here, and
    // the subscription is held until the exit button gives it back.
    _ota.Subscribe();

    PrepareContainer();

    _itemCount = 0;
    _itemIndex = 0;

    ApplyItemsToSlots();
    HideSelectList();

    StartScan();
}

/**
 * @brief Move to the next discovered device.
 * @param e Unused.
 */
void UpdateScreenHandler::OnNextUpdateItem(lv_event_t *e) {
    (void) e;
    if (_itemIndex + 1 >= _itemCount) {
        LOG_DEBUG("Already at last update item", _itemIndex);
        return;
    }
    _itemIndex++;
    LOG_INFO("Next update item", _itemIndex, _items[_itemIndex].name);
    Refresh();
}

/**
 * @brief Move to the previous discovered device.
 * @param e Unused.
 */
void UpdateScreenHandler::OnPreviousUpdateItem(lv_event_t *e) {
    (void) e;
    if (_itemIndex <= 0) {
        LOG_DEBUG("Already at first update item", _itemIndex);
        return;
    }
    _itemIndex--;
    LOG_INFO("Previous update item", _itemIndex, _items[_itemIndex].name);
    Refresh();
}

/**
 * @brief Select the pressed device and arm its update.
 * @param e Carries the widget that was pressed.
 * @note The flow changes screen once this returns, so everything set here
 *       is in place before the update screen appears.
 */
void UpdateScreenHandler::OnUpdateItemPressed(lv_event_t *e) {
    const int index = SlotIndexForObject((lv_obj_t *) lv_event_get_target(e));
    if (index < 0 || index >= _itemCount) {
        LOG_WARN("Update pressed but no matching item slot", index);
        return;
    }

    _selectedIndex = index;
    _selectedId = _items[index].id;
    LOG_INFO("Update requested for", _items[index].name, _selectedId);

    ShowSelectedOnUpdateScreen();
    BeginUpdate();
}

/**
 * @brief Abandon whatever is running and go back to the list.
 * @param e Unused.
 */
void UpdateScreenHandler::OnReturnToUpdateSelect(lv_event_t *e) {
    (void) e;
    LOG_INFO("Leaving update screen, back to update select");

    StopUpdate();

    _selectedId = -1;
    _selectedIndex = -1;
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

    if (_scanTimer != nullptr) {
        lv_timer_delete(_scanTimer);
        _scanTimer = nullptr;
    }
    _scanning = false;

    CancelLoadingReveal();
    CancelDoneRescan();

    _selectedId = -1;
    _selectedIndex = -1;
}

/**
 * @brief Broadcast discovery and collect answers for SCAN_DURATION_MS.
 * @note This display is added by hand at index 0: it cannot answer its own
 *       broadcast.
 */
void UpdateScreenHandler::StartScan() {
    if (_scanTimer != nullptr) {
        lv_timer_delete(_scanTimer);
        _scanTimer = nullptr;
    }

    _scanning = true;
    _itemCount = 0;
    _itemIndex = 0;

    AddItem((int) ECanNode::NODE_AIRRIDE_GUI, NodeName(ECanNode::NODE_AIRRIDE_GUI));

    _ota.SendDiscover();

    _scanTimer = lv_timer_create(ScanTimerCb, SCAN_DURATION_MS, this);
    lv_timer_set_repeat_count(_scanTimer, 1);
}

/**
 * @brief Timer callback closing the discovery window.
 * @param timer Carries the handler.
 */
void UpdateScreenHandler::ScanTimerCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);
    self->_scanTimer = nullptr;
    self->OnScanFinished();
}

/**
 * @brief Show whatever answered and hide the spinner.
 */
void UpdateScreenHandler::OnScanFinished() {
    _scanning = false;
    _itemIndex = 0;
    LOG_INFO("Scan finished, devices found:", _itemCount);

    ApplyItemsToSlots();
    SetLoadingVisible(false);
    Refresh();
}

/**
 * @brief Add a node that answered discovery.
 * @param node The node that answered. Ignored unless a scan is open.
 */
void UpdateScreenHandler::AddNode(ECanNode node) {
    if (!_scanning) {
        LOG_DEBUG("Discovery answer outside a scan, ignoring node", (int) node);
        return;
    }
    AddItem((int) node, NodeName(node));
}

/**
 * @brief Append a device to the list.
 * @param id   ECanNode of the device.
 * @param name What to call it on screen.
 * @return false when it is a duplicate or there is no free slot.
 */
bool UpdateScreenHandler::AddItem(int id, const char *name) {
    for (int i = 0; i < _itemCount; i++) {
        if (_items[i].id == id) {
            return false;
        }
    }

    if (_itemCount >= MAX_ITEMS) {
        LOG_WARN("Dropping device, no free UpdateItem slot for", name);
        return false;
    }

    UpdatebleItem &item = _items[_itemCount];
    item.id = id;
    strncpy(item.name, name, sizeof(item.name) - 1);
    item.name[sizeof(item.name) - 1] = '\0';

    _itemCount++;
    LOG_DEBUG("Update item added", id, name);
    return true;
}

/**
 * @brief Display name for a node.
 * @param node The node.
 * @return A static string.
 */
const char *UpdateScreenHandler::NodeName(ECanNode node) {
    switch (node) {
        case ECanNode::NODE_AIRRIDE_CONTROLLER:
            return "Controller";
        case ECanNode::NODE_AIRRIDE_GUI:
            return "Display";
        default:
            return "Unknown";
    }
}

/**
 * @brief Arm the update; START is sent from StartTimerCb.
 * @warning Not sent here. This runs inside the button press and the flow
 *          changes screen only once it returns, so bringing up the WiFi
 *          access point here would block with the old screen still up.
 */
void UpdateScreenHandler::BeginUpdate() {
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
 * @param timer Carries the handler.
 */
void UpdateScreenHandler::StartTimerCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);
    self->_startTimer = nullptr;

    if (!self->_updating) {
        LOG_DEBUG("Update cancelled before it started");
        return;
    }

    LOG_INFO("Starting update on node", self->_selectedId);
    self->_lastStatusMs = lv_tick_get();
    self->_ota.SendStart((ECanNode) self->_selectedId);
}

/**
 * @brief Drop a pending automatic return to the device list.
 */
void UpdateScreenHandler::CancelDoneRescan() {
    if (_doneTimer != nullptr) {
        lv_timer_delete(_doneTimer);
        _doneTimer = nullptr;
    }
}

/**
 * @brief Return to the device list once a finished update has been read.
 * @param timer Carries the handler.
 * @note The only place this handler changes screen itself; everywhere else
 *       that belongs to the flow, driven by a button.
 */
void UpdateScreenHandler::DoneRescanCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);
    self->_doneTimer = nullptr;

    LOG_INFO("Update finished, back to the device list");
    self->_selectedId = -1;
    self->_selectedIndex = -1;

    loadScreenNoAnim(SCREEN_ID_UPDATE_SELECT_SCREEN);
}

/**
 * @brief Stop whatever is running and drop the timers.
 * @note The stop is broadcast: a node that took the START but never
 *       reported is exactly the one that has to hear it.
 */
void UpdateScreenHandler::StopUpdate() {
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
 * @param timer Carries the handler.
 */
void UpdateScreenHandler::UpdatePumpCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);

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
 * @brief Handle one status, from a remote node or the local access point.
 * @param node   Who reported.
 * @param status Type, phase and progress.
 */
void UpdateScreenHandler::OnOtaStatus(ECanNode node, const CANAirRideOTAStatus &status) {
    switch (status.type) {
        case EOTAStatusType::AVAILABLE:
            AddNode(node);
            break;

        case EOTAStatusType::UPDATE:
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
            break;

        default:
            LOG_ERROR("Unknown OTA status type", (uint8_t) status.type);
            break;
    }
}

/**
 * @brief Put the last status on screen: phase in words, progress on the bar.
 * @note Repaints immediately during a self update, because ArduinoOTA
 *       blocks the loop for the whole image and lv_timer_handler() would
 *       never get another turn. Only safe from inside the pump.
 */
void UpdateScreenHandler::ShowStatus() {
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

    SetUpdateStatus(phase);
    SetUpdateProgress(_lastStatus.progress);
    ShowRetry(_lastStatus.phase == EOTAUpdatePhase::ERROR);

    if (_ota.IsPumping()) {
        lv_refr_now(NULL);
    }
}

/**
 * @brief The generated widgets for one list slot.
 * @param index Slot index.
 * @return The slot, all null when out of range.
 */
UpdateScreenHandler::ItemSlot UpdateScreenHandler::SlotAt(int index) {
    switch (index) {
        case 0:
            return {objects.update_item0,
                    objects.update_item0__update_itemtext,
                    objects.update_item0__update_item_update_button};
        case 1:
            return {objects.update_item1,
                    objects.update_item1__update_itemtext,
                    objects.update_item1__update_item_update_button};
        case 2:
            return {objects.update_item2,
                    objects.update_item2__update_itemtext,
                    objects.update_item2__update_item_update_button};
        case 3:
            return {objects.update_item3,
                    objects.update_item3__update_itemtext,
                    objects.update_item3__update_item_update_button};
        case 4:
            return {objects.update_item4,
                    objects.update_item4__update_itemtext,
                    objects.update_item4__update_item_update_button};
        default:
            return {nullptr, nullptr, nullptr};
    }
}

/**
 * @brief Which slot an object belongs to.
 * @param obj Object to trace.
 * @return The slot index, or -1.
 */
int UpdateScreenHandler::SlotIndexForObject(lv_obj_t *obj) {
    for (lv_obj_t *node = obj; node != nullptr; node = lv_obj_get_parent(node)) {
        for (int i = 0; i < MAX_ITEMS; i++) {
            const ItemSlot slot = SlotAt(i);
            if (node == slot.button || node == slot.root) {
                return i;
            }
        }
    }
    return -1;
}

/**
 * @brief Set up list scrolling. Runs once.
 */
void UpdateScreenHandler::PrepareContainer() {
    if (_containerReady) {
        return;
    }

    lv_obj_set_scroll_snap_x(objects.update_item_container, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_event_cb(objects.update_item_container, ContainerScrollEndCb,
                        LV_EVENT_SCROLL_END, nullptr);

    _containerReady = true;
}

/**
 * @brief Adopt the slot a finger drag settled on.
 * @param e Unused.
 */
void UpdateScreenHandler::ContainerScrollEndCb(lv_event_t *e) {
    (void) e;
    UpdateScreenHandler *self = Active();
    if (self == nullptr || self->_itemCount <= 0) {
        return;
    }

    lv_obj_t *cont = objects.update_item_container;
    const int32_t scrollX = lv_obj_get_scroll_x(cont);

    int nearest = 0;
    int32_t nearestDistance = INT32_MAX;
    for (int i = 0; i < self->_itemCount; i++) {
        const ItemSlot slot = SlotAt(i);
        if (slot.root == nullptr) {
            continue;
        }
        int32_t distance = lv_obj_get_x(slot.root) - scrollX;
        if (distance < 0) {
            distance = -distance;
        }
        if (distance < nearestDistance) {
            nearestDistance = distance;
            nearest = i;
        }
    }

    if (nearest == self->_itemIndex) {
        return;
    }

    self->_itemIndex = nearest;
    LOG_DEBUG("Scrolled to update item", self->_itemIndex);
    self->RefreshNavButtons();
}

/**
 * @brief Fill the slots from the item list and hide the unused ones.
 */
void UpdateScreenHandler::ApplyItemsToSlots() {
    for (int i = 0; i < MAX_ITEMS; i++) {
        const ItemSlot slot = SlotAt(i);
        if (slot.root == nullptr) {
            continue;
        }

        if (i < _itemCount) {
            if (slot.nameLabel != nullptr) {
                lv_label_set_text(slot.nameLabel, _items[i].name);
            }
            lv_obj_remove_flag(slot.root, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(slot.root, LV_OBJ_FLAG_HIDDEN);
        }
    }
}

/**
 * @brief Put the selected device on the update screen before it is shown.
 */
void UpdateScreenHandler::ShowSelectedOnUpdateScreen() {
    if (_selectedIndex >= 0 && _selectedIndex < _itemCount) {
        lv_label_set_text(objects.update_screen_item_name, _items[_selectedIndex].name);
    } else {
        lv_label_set_text(objects.update_screen_item_name, "");
    }

    SetUpdateProgress(0);
    SetUpdateStatus("IDLE");
    ShowRetry(false);
}

/**
 * @brief Swap the update screen between its two exits.
 * @param retry true after a failure: back is hidden and retry takes its
 *              place. Both run the same action and screen change.
 */
void UpdateScreenHandler::ShowRetry(bool retry) {
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
void UpdateScreenHandler::SetUpdateProgress(int percent) {
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
void UpdateScreenHandler::SetUpdateStatus(const char *status) {
    lv_label_set_text(objects.update_screen_item_status, status);
}

/**
 * @brief Put the select screen back to spinner, no list.
 */
void UpdateScreenHandler::HideSelectList() {
    lv_obj_add_flag(objects.update_item_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.previous_item, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.next_item, LV_OBJ_FLAG_HIDDEN);
    ScheduleLoadingReveal();
}

/**
 * @brief Show or hide the list and the previous/next arrows.
 */
void UpdateScreenHandler::RefreshNavButtons() {
    const bool hasItems = !_scanning && _itemCount > 0;

    if (hasItems) {
        lv_obj_remove_flag(objects.update_item_container, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.update_item_container, LV_OBJ_FLAG_HIDDEN);
    }

    if (hasItems && _itemIndex > 0) {
        lv_obj_remove_flag(objects.previous_item, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.previous_item, LV_OBJ_FLAG_HIDDEN);
    }

    if (hasItems && _itemIndex + 1 < _itemCount) {
        lv_obj_remove_flag(objects.next_item, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.next_item, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Refresh the arrows and scroll the selected item into view.
 */
void UpdateScreenHandler::Refresh() {
    RefreshNavButtons();

    if (_scanning || _itemCount <= 0) {
        return;
    }

    const ItemSlot slot = SlotAt(_itemIndex);
    if (slot.root != nullptr) {
        lv_obj_scroll_to_view(slot.root, LV_ANIM_ON);
    }
}

/**
 * @brief Show or hide the spinner.
 * @param visible Whether it should be shown.
 */
void UpdateScreenHandler::SetLoadingVisible(bool visible) {
    if (visible) {
        lv_obj_remove_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Drop a pending spinner reveal.
 */
void UpdateScreenHandler::CancelLoadingReveal() {
    if (_loadingRevealTimer != nullptr) {
        lv_timer_delete(_loadingRevealTimer);
        _loadingRevealTimer = nullptr;
    }
}

/**
 * @brief Hide the spinner now and bring it back once the screen is painted.
 * @note A spinner animates, so its area is already invalid before the
 *       screen change invalidates everything, and it would reach the glass
 *       ahead of the screen it sits on.
 */
void UpdateScreenHandler::ScheduleLoadingReveal() {
    SetLoadingVisible(false);

    _loadingRevealTimer = lv_timer_create(LoadingRevealCb, LOADING_REVEAL_MS, this);
    lv_timer_set_repeat_count(_loadingRevealTimer, 1);
    LOG_DEBUG("Spinner hidden, revealing in ms:", LOADING_REVEAL_MS);
}

/**
 * @brief Reveal the spinner, unless the scan already finished.
 * @param timer Carries the handler.
 */
void UpdateScreenHandler::LoadingRevealCb(lv_timer_t *timer) {
    UpdateScreenHandler *self = (UpdateScreenHandler *) lv_timer_get_user_data(timer);

    self->_loadingRevealTimer = nullptr;

    if (!self->_scanning) {
        LOG_DEBUG("Spinner reveal skipped, scan already finished");
        return;
    }

    LOG_DEBUG("Spinner revealed");
    lv_obj_remove_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
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
