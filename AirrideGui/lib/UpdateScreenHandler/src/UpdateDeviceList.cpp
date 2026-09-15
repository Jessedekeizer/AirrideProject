#include "UpdateDeviceList.h"

#include <stdint.h>
#include <string.h>

#include "Logger.h"
#include "screens.h"

/**
 * @brief Drop a pending spinner reveal.
 */
UpdateDeviceList::~UpdateDeviceList() {
    CancelLoadingReveal();
}

/**
 * @brief Set up list scrolling. Runs once.
 */
void UpdateDeviceList::Prepare() {
    if (_containerReady) {
        return;
    }

    lv_obj_set_scroll_snap_x(objects.update_item_container, LV_SCROLL_SNAP_CENTER);
    lv_obj_add_event_cb(objects.update_item_container, ContainerScrollEndCb,
                        LV_EVENT_SCROLL_END, this);

    _containerReady = true;
}

/**
 * @brief Empty the list and show the spinner, ready for answers.
 * @note This display is added by hand: it cannot answer its own broadcast.
 */
void UpdateDeviceList::BeginScan() {
    _scanning = true;
    _itemCount = 0;
    _itemIndex = 0;

    ApplyItemsToSlots();
    HideSelectList();

    AddItem((int) ECanNode::NODE_AIRRIDE_GUI, NodeName(ECanNode::NODE_AIRRIDE_GUI));
}

/**
 * @brief Show whatever answered and hide the spinner.
 */
void UpdateDeviceList::EndScan() {
    _scanning = false;
    _itemIndex = 0;
    LOG_INFO("Scan finished, devices found:", _itemCount);

    ApplyItemsToSlots();
    SetLoadingVisible(false);
    Refresh();
}

/**
 * @brief Give up on a scan without showing what it found.
 */
void UpdateDeviceList::AbortScan() {
    _scanning = false;
    CancelLoadingReveal();
}

/**
 * @brief Add a node that answered discovery.
 * @param node The node that answered. Ignored unless a scan is open.
 * @return false when it was ignored, a duplicate, or there was no free slot.
 */
bool UpdateDeviceList::Add(ECanNode node) {
    if (!_scanning) {
        LOG_DEBUG("Discovery answer outside a scan, ignoring node", (int) node);
        return false;
    }
    return AddItem((int) node, NodeName(node));
}

/**
 * @brief Append a device to the list.
 * @param id   ECanNode of the device.
 * @param name What to call it on screen.
 * @return false when it is a duplicate or there is no free slot.
 */
bool UpdateDeviceList::AddItem(int id, const char *name) {
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
const char *UpdateDeviceList::NodeName(ECanNode node) {
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
 * @brief Move to the next discovered device.
 */
void UpdateDeviceList::Next() {
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
 */
void UpdateDeviceList::Previous() {
    if (_itemIndex <= 0) {
        LOG_DEBUG("Already at first update item", _itemIndex);
        return;
    }
    _itemIndex--;
    LOG_INFO("Previous update item", _itemIndex, _items[_itemIndex].name);
    Refresh();
}

/**
 * @brief Select the device whose slot an object belongs to.
 * @param obj The object that was pressed.
 * @return false when the object is not part of a filled slot.
 */
bool UpdateDeviceList::SelectFromObject(lv_obj_t *obj) {
    const int index = SlotIndexForObject(obj);
    if (index < 0 || index >= _itemCount) {
        LOG_WARN("Update pressed but no matching item slot", index);
        return false;
    }

    _selectedIndex = index;
    _selectedId = _items[index].id;
    return true;
}

/**
 * @brief Forget which device was selected.
 */
void UpdateDeviceList::ClearSelection() {
    _selectedId = -1;
    _selectedIndex = -1;
}

/**
 * @brief The name of the selected device.
 * @return The name, or an empty string when nothing is selected.
 */
const char *UpdateDeviceList::SelectedName() const {
    if (_selectedIndex < 0 || _selectedIndex >= _itemCount) {
        return "";
    }
    return _items[_selectedIndex].name;
}

/**
 * @brief The generated widgets for one list slot.
 * @param index Slot index.
 * @return The slot, all null when out of range.
 */
UpdateDeviceList::ItemSlot UpdateDeviceList::SlotAt(int index) {
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
int UpdateDeviceList::SlotIndexForObject(lv_obj_t *obj) {
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
 * @brief Adopt the slot a finger drag settled on.
 * @param e Carries the list.
 */
void UpdateDeviceList::ContainerScrollEndCb(lv_event_t *e) {
    UpdateDeviceList *self = (UpdateDeviceList *) lv_event_get_user_data(e);
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
void UpdateDeviceList::ApplyItemsToSlots() {
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
 * @brief Put the select screen back to spinner, no list.
 */
void UpdateDeviceList::HideSelectList() {
    lv_obj_add_flag(objects.update_item_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.previous_item, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(objects.next_item, LV_OBJ_FLAG_HIDDEN);
    ScheduleLoadingReveal();
}

/**
 * @brief Show or hide the list and the previous/next arrows.
 */
void UpdateDeviceList::RefreshNavButtons() {
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
void UpdateDeviceList::Refresh() {
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
void UpdateDeviceList::SetLoadingVisible(bool visible) {
    if (visible) {
        lv_obj_remove_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
    } else {
        lv_obj_add_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
    }
}

/**
 * @brief Drop a pending spinner reveal.
 */
void UpdateDeviceList::CancelLoadingReveal() {
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
void UpdateDeviceList::ScheduleLoadingReveal() {
    SetLoadingVisible(false);

    CancelLoadingReveal();
    _loadingRevealTimer = lv_timer_create(LoadingRevealCb, LOADING_REVEAL_MS, this);
    lv_timer_set_repeat_count(_loadingRevealTimer, 1);
    LOG_DEBUG("Spinner hidden, revealing in ms:", LOADING_REVEAL_MS);
}

/**
 * @brief Reveal the spinner, unless the scan already finished.
 * @param timer Carries the list.
 */
void UpdateDeviceList::LoadingRevealCb(lv_timer_t *timer) {
    UpdateDeviceList *self = (UpdateDeviceList *) lv_timer_get_user_data(timer);

    self->_loadingRevealTimer = nullptr;

    if (!self->_scanning) {
        LOG_DEBUG("Spinner reveal skipped, scan already finished");
        return;
    }

    LOG_DEBUG("Spinner revealed");
    lv_obj_remove_flag(objects.update_select_loading, LV_OBJ_FLAG_HIDDEN);
}
