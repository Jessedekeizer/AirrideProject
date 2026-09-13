#ifndef UPDATESCREENHANDLER_H
#define UPDATESCREENHANDLER_H

#include <lvgl.h>

#include "CanMessageIds.h"
#include "CanMessages.h"
#include "OTACommunication.h"
#include "UpdateDeviceList.h"
#include "UpdateRunner.h"

/**
 * @brief The two update screens: what the buttons do, and the scan.
 *
 * Holds the discovery window open and routes bus status to whichever of the
 * two below it belongs to. The carousel is UpdateDeviceList, one update from
 * START to done is UpdateRunner.
 */
class UpdateScreenHandler {
public:
    explicit UpdateScreenHandler(OTACommunication &ota);

    ~UpdateScreenHandler();

    static UpdateScreenHandler *Active();

    void Begin();

    void OnUpdateSelectScreenLoaded(lv_event_t *e);

    void OnNextUpdateItem(lv_event_t *e);

    void OnPreviousUpdateItem(lv_event_t *e);

    void OnUpdateItemPressed(lv_event_t *e);

    void OnReturnToUpdateSelect(lv_event_t *e);

    void OnUpdateSelectToMain(lv_event_t *e);

private:
    UpdateScreenHandler(const UpdateScreenHandler &) = delete;
    UpdateScreenHandler &operator=(const UpdateScreenHandler &) = delete;

    static constexpr uint32_t SCAN_DURATION_MS = 3000;

    void StartScan();

    void StopScanTimer();

    static void ScanTimerCb(lv_timer_t *timer);

    void OnOtaStatus(ECanNode node, const CANAirRideOTAStatus &status);

    static UpdateScreenHandler *_active;

    OTACommunication &_ota;
    UpdateDeviceList _list;
    UpdateRunner _runner;

    lv_timer_t *_scanTimer = nullptr;
};

#endif
