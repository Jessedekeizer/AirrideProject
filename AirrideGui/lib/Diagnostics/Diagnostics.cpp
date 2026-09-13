#include "Diagnostics.h"

#if DIAG_HEAP

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace {
    constexpr uint32_t HEAP_REPORT_INTERVAL_MS = 1000;

    uint32_t lastReportMs = 0;
}

/**
 * @brief Print one heap line, tagged with where it was taken.
 * @param when Label for the call site.
 * @note Read the 8-bit figures. malloc() serves only byte-addressable memory,
 *       while MALLOC_CAP_INTERNAL also counts IRAM no allocation can use, so
 *       the internal numbers read healthier than the chip is.
 * @note The stack figure is the low-water mark of the calling task, so from
 *       loop() it describes loopTask, the one LVGL renders on. ESP-IDF's
 *       uxTaskGetStackHighWaterMark() returns bytes, not words as vanilla
 *       FreeRTOS does.
 */
void Diagnostics::ReportHeap(const char *when) {
    Serial.printf("HEAP %s | 8bit free %u, largest %u, min %u | internal free %u, largest %u | stack min %u\n",
                  when,
                  (unsigned) heap_caps_get_free_size(MALLOC_CAP_8BIT),
                  (unsigned) heap_caps_get_largest_free_block(MALLOC_CAP_8BIT),
                  (unsigned) heap_caps_get_minimum_free_size(MALLOC_CAP_8BIT),
                  (unsigned) heap_caps_get_free_size(MALLOC_CAP_INTERNAL),
                  (unsigned) heap_caps_get_largest_free_block(MALLOC_CAP_INTERNAL),
                  (unsigned) uxTaskGetStackHighWaterMark(nullptr));
}

/**
 * @brief Print a heap line at a fixed interval. Call every loop.
 */
void Diagnostics::HeapTick() {
    const uint32_t now = millis();
    if (now - lastReportMs < HEAP_REPORT_INTERVAL_MS) {
        return;
    }
    lastReportMs = now;
    ReportHeap("idle");
}

#endif
