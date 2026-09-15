#ifndef UPDATERUNNER_H
#define UPDATERUNNER_H

#include <lvgl.h>

#include "CanMessageIds.h"
#include "CanMessages.h"
#include "OTACommunication.h"
#include "UpdateDeviceList.h"

/**
 * @brief One update from START to done, and the update screen it shows on.
 *
 * Owns the timers that arm, time out and finish an update, the last status
 * heard, and the progress bar, status line and retry button. Clears the
 * list's selection on the way back, which is the only thing it asks of it.
 */
class UpdateRunner {
public:
    UpdateRunner(OTACommunication &ota, UpdateDeviceList &list);

    ~UpdateRunner();

    void Begin(int nodeId, const char *name);

    void Stop();

    void OnStatus(const CANAirRideOTAStatus &status);

    void CancelDoneRescan();

    bool IsUpdating() const { return _updating; }

private:
    UpdateRunner(const UpdateRunner &) = delete;
    UpdateRunner &operator=(const UpdateRunner &) = delete;

    static constexpr uint32_t UPDATE_TIMEOUT_MS = 15000;

    static constexpr uint32_t UPDATE_PUMP_MS = 250;

    static constexpr uint32_t DONE_RESCAN_MS = 5000;

    static constexpr uint32_t START_DELAY_MS = 200;

    static void StartTimerCb(lv_timer_t *timer);

    static void UpdatePumpCb(lv_timer_t *timer);

    static void DoneRescanCb(lv_timer_t *timer);

    void ShowStatus();

    void ShowSelected(const char *name);

    static void ShowRetry(bool retry);

    static void SetProgress(int percent);

    static void SetStatusText(const char *status);

    OTACommunication &_ota;
    UpdateDeviceList &_list;

    int _nodeId = -1;
    bool _updating = false;
    lv_timer_t *_updateTimer = nullptr;
    lv_timer_t *_startTimer = nullptr;
    lv_timer_t *_doneTimer = nullptr;
    uint32_t _lastStatusMs = 0;
    CANAirRideOTAStatus _lastStatus = {};
};

#endif
