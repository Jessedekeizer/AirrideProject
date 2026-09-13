#ifndef UPDATEDEVICELIST_H
#define UPDATEDEVICELIST_H

#include <lvgl.h>

#include "CanMessageIds.h"

struct UpdatebleItem {
    int id;
    char name[32];
};

/**
 * @brief The five slot carousel of devices that can be updated.
 *
 * Owns what was found and which one is in front, and drives the generated
 * slot widgets, the arrows and the spinner. Knows nothing about the bus or
 * about updating - the handler scans, and hands answers here.
 */
class UpdateDeviceList {
public:
    static constexpr int MAX_ITEMS = 5;

    UpdateDeviceList() = default;

    ~UpdateDeviceList();

    void Prepare();

    void BeginScan();

    void EndScan();

    void AbortScan();

    bool Add(ECanNode node);

    void Next();

    void Previous();

    bool SelectFromObject(lv_obj_t *obj);

    void ClearSelection();

    int Count() const { return _itemCount; }

    bool Scanning() const { return _scanning; }

    int SelectedId() const { return _selectedId; }

    const char *SelectedName() const;

private:
    struct ItemSlot {
        lv_obj_t *root;
        lv_obj_t *nameLabel;
        lv_obj_t *button;
    };

    UpdateDeviceList(const UpdateDeviceList &) = delete;
    UpdateDeviceList &operator=(const UpdateDeviceList &) = delete;

    static constexpr uint32_t LOADING_REVEAL_MS = 250;

    bool AddItem(int id, const char *name);

    static const char *NodeName(ECanNode node);

    static ItemSlot SlotAt(int index);

    static int SlotIndexForObject(lv_obj_t *obj);

    static void ContainerScrollEndCb(lv_event_t *e);

    void ApplyItemsToSlots();

    void HideSelectList();

    void RefreshNavButtons();

    void Refresh();

    void SetLoadingVisible(bool visible);

    void ScheduleLoadingReveal();

    void CancelLoadingReveal();

    static void LoadingRevealCb(lv_timer_t *timer);

    UpdatebleItem _items[MAX_ITEMS] = {};
    int _itemCount = 0;
    int _itemIndex = 0;
    int _selectedId = -1;
    int _selectedIndex = -1;
    bool _scanning = false;
    bool _containerReady = false;
    lv_timer_t *_loadingRevealTimer = nullptr;
};

#endif
