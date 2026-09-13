#include <Arduino.h>
#include <SPI.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "ui.h"
#include "CalibrationScreenHandler.h"
#include "CanBus.h"
#include "Communication.h"
#include "Diagnostics.h"
#include "LargeCanMessageHandler.h"
#include "LogStorage.h"
#include "MainScreenCommunication.h"
#include "MainScreenData.h"
#include "MainScreenHandler.h"
#include "OTACommunication.h"
#include "SdCardService.h"
#include "screens.h"
#include "SettingsDevice.h"
#include "SettingsScreenHandler.h"
#include "SettingsStorage.h"
#include "UpdateScreenHandler.h"
#include <XPT2046_Bitbang.h>

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

#define CAN_TX_PIN 22
#define CAN_RX_PIN 27

#define SERIAL_BAUD_RATE 115200

#define TFT_ROTATION 1

static const uint16_t screenWidth = 320;
static const uint16_t screenHeight = 240;

#define UI_TICK_INTERVAL_MS 16

#define LV_HANDLER_MAX_WAIT_MS 5

SPIClass mySpi = SPIClass(VSPI);

XPT2046_Bitbang touchScreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight);

SettingsDevice deviceSettings;

SdCardService sdCardService(SS, mySpi);
SettingsStorage settingsStorage(sdCardService);
LogStorage logStorage(sdCardService);
SettingsScreenHandler settingsScreenHandler(deviceSettings, settingsStorage);
CalibrationScreenHandler calibrationScreenHandler(deviceSettings, settingsStorage, touchScreen);

CanBus canBus;
LargeCanMessageHandler largeCanMessageHandler;
Communication communication(largeCanMessageHandler, ECanNode::NODE_AIRRIDE_GUI);

MainScreenData mainScreenData;
MainScreenCommunication mainScreenCommunication(communication, mainScreenData, logStorage, deviceSettings);
MainScreenHandler mainScreenHandler(mainScreenData, mainScreenCommunication, deviceSettings);

OTACommunication otaCommunication(communication);
UpdateScreenHandler updateScreenHandler(otaCommunication);

static const uint32_t DRAW_BUF_CANDIDATES_PX[] = {
    screenWidth * screenHeight / 5,
    screenWidth * screenHeight / 7,
};

#define MIN_FREE_8BIT_AFTER_BUFFERS (156 * 1024U)

static uint8_t *buf1 = nullptr;
static uint8_t *buf2 = nullptr;
static uint32_t drawBufBytes = 0;

static lv_display_t *display = nullptr;

static bool singleBuffered = false;

static uint32_t last_ui_tick_ms = 0;
static uint32_t next_lv_handler_ms = 0;

#if LV_USE_LOG != 0
/**
 * @brief Print callback for LVGL's own log module.
 * @param level LVGL severity.
 * @param buf   Message text.
 * @note Compiled in only when LV_USE_LOG is on, which is a debugging build.
 */
void LvglPrint(lv_log_level_t level, const char *buf)
{
    const char *name = "LVGL";
    switch (level)
    {
    case LV_LOG_LEVEL_TRACE: name = "LVGL TRACE"; break;
    case LV_LOG_LEVEL_INFO:  name = "LVGL INFO";  break;
    case LV_LOG_LEVEL_WARN:  name = "LVGL WARN";  break;
    case LV_LOG_LEVEL_ERROR: name = "LVGL ERROR"; break;
    case LV_LOG_LEVEL_USER:  name = "LVGL USER";  break;
    default: break;
    }

    Serial.print("[");
    Serial.print(name);
    Serial.print("] ");
    Serial.println(buf);
    Serial.flush();
}
#endif

/**
 * @brief Push one rendered area to the panel over DMA.
 * @param disp   Display being flushed.
 * @param area   Region rendered.
 * @param px_map Pixel data, already in the panel's byte order.
 * @note Returns before the transfer completes so rendering overlaps it; the
 *       next call's dmaWait() closes it. Single-buffered, that overlap is gone
 *       and the wait happens here instead.
 */
void DisplayFlush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    tft.dmaWait();

    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);
    tft.pushPixelsDMA((uint16_t *)px_map, w * h);

    if (singleBuffered)
    {
        tft.dmaWait();
    }

    lv_display_flush_ready(disp);
}

/**
 * @brief Read the touch controller for LVGL.
 * @param indev Input device being read.
 * @param data  Filled with the press state and point.
 * @note Coordinates are clamped: the calibration maps one past the last pixel,
 *       so an edge press reports x = 320 on a 0..319 display.
 */
void TouchpadRead(lv_indev_t *indev, lv_indev_data_t *data)
{
    const TouchPoint touch = touchScreen.getTouch();
    if (touch.zRaw == 0)
    {
        data->state = LV_INDEV_STATE_RELEASED;
        return;
    }

    int32_t x = touch.x;
    int32_t y = touch.y;
    if (x < 0) x = 0;
    if (x > screenWidth - 1) x = screenWidth - 1;
    if (y < 0) y = 0;
    if (y > screenHeight - 1) y = screenHeight - 1;

    data->state = LV_INDEV_STATE_PRESSED;
    data->point.x = x;
    data->point.y = y;
}

/**
 * @brief Release the heap a WiFi access point needs, before it is started.
 *
 * Frees the second draw buffer and stops CAN. Measured: the AP takes 58,208
 * bytes against roughly 46,800 free at idle, and esp_phy_load_cal_and_init()
 * calls abort() rather than failing when it is short.
 *
 * @warning SetQueues(nullptr) must precede canBus.Stop() - loop() reads those
 *          handles every iteration and the queues are about to be destroyed.
 * @note Nothing is restored; abandoning a self update reboots instead.
 */
static void FreeMemoryForSelfOta()
{
    Serial.println("Self update: releasing memory for the WiFi AP");

    if (!singleBuffered && buf2 != nullptr)
    {
        tft.dmaWait();

        lv_display_set_buffers(display, buf1, nullptr, drawBufBytes, LV_DISPLAY_RENDER_MODE_PARTIAL);
        singleBuffered = true;

        heap_caps_free(buf2);
        buf2 = nullptr;
    }

    communication.SetQueues(nullptr, nullptr);
    canBus.Stop();

    DIAG_HEAP_REPORT("before WiFi");
}

/**
 * @brief Serial, LVGL core, touch controller and panel.
 */
static void InitializeHardware()
{
    Serial.begin(SERIAL_BAUD_RATE);

    String LVGL_Arduino = "Hello Arduino! ";
    LVGL_Arduino += String('V') + lv_version_major() + "." + lv_version_minor() + "." + lv_version_patch();

    Serial.println(LVGL_Arduino);
    Serial.println("I am LVGL_Arduino");

    lv_init();
    lv_tick_set_cb((lv_tick_get_cb_t)millis);

#if LV_USE_LOG != 0
    lv_log_register_print_cb(LvglPrint);
#endif

    touchScreen.begin();
    touchScreen.setCalibration(240, 3800, 200, 3700);

    tft.begin();
    tft.initDMA();
}

/**
 * @brief Take the largest pair of draw buffers that still leaves a usable heap.
 * @return false when even the smallest candidate cannot be allocated.
 * @note Candidates are tried largest first. All but the last must also leave
 *       MIN_FREE_8BIT_AFTER_BUFFERS behind, or glyph allocation fails later.
 */
static bool AllocateDrawBuffers()
{
    const size_t candidateCount = sizeof(DRAW_BUF_CANDIDATES_PX) / sizeof(DRAW_BUF_CANDIDATES_PX[0]);
    for (size_t i = 0; i < candidateCount; i++)
    {
        const uint32_t bytes = DRAW_BUF_CANDIDATES_PX[i] * 2;
        buf1 = (uint8_t *)heap_caps_malloc(bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
        buf2 = (uint8_t *)heap_caps_malloc(bytes, MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);

        if (buf1 != nullptr && buf2 != nullptr)
        {
            const size_t freeAfter = heap_caps_get_free_size(MALLOC_CAP_8BIT);
            const bool lastCandidate = (i + 1 == candidateCount);
            if (lastCandidate || freeAfter >= MIN_FREE_8BIT_AFTER_BUFFERS)
            {
                drawBufBytes = bytes;
                break;
            }
            Serial.printf("Draw buffers of %u bytes would leave only %u free, trying smaller\n",
                          (unsigned)bytes, (unsigned)freeAfter);
        }

        heap_caps_free(buf1);
        heap_caps_free(buf2);
        buf1 = nullptr;
        buf2 = nullptr;
    }

    if (drawBufBytes == 0)
    {
        return false;
    }

    Serial.printf("Draw buffers %u bytes each, %u transfers per full screen\n",
                  (unsigned)drawBufBytes,
                  (unsigned)((screenWidth * screenHeight * 2 + drawBufBytes - 1) / drawBufBytes));

    DIAG_HEAP_REPORT("after draw buffers");
    return true;
}

/**
 * @brief Register the display and the touch input device with LVGL.
 */
static void InitializeDisplay()
{
    tft.setRotation(TFT_ROTATION);
    tft.invertDisplay(true);

    display = lv_display_create(screenWidth, screenHeight);
    lv_display_set_flush_cb(display, DisplayFlush);

    lv_display_set_color_format(display, LV_COLOR_FORMAT_RGB565_SWAPPED);
    lv_display_set_buffers(display, buf1, buf2, drawBufBytes, LV_DISPLAY_RENDER_MODE_PARTIAL);

    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev, TouchpadRead);
}

/**
 * @brief Bring up the SD card and load the stored settings and calibration.
 */
static void InitializeStorage()
{
    if (!sdCardService.Begin())
    {
        Serial.println("SD card not ready, settings will not persist");
    }

    settingsStorage.Begin();
    settingsScreenHandler.Load();
    calibrationScreenHandler.ApplyStoredCalibration();
}

/**
 * @brief Start CAN and connect the screen handlers to it.
 * @warning Must run after InitializeStorage(): the main screen sends the
 *          settings to the controller as soon as it loads.
 * @note Nothing subscribes here. Each screen takes its subscription on its
 *       load start event and gives it back on unload start, so a screen
 *       that is not up decodes nothing.
 */
static void InitializeBus()
{
    canBus.Setup(CAN_TX_PIN, CAN_RX_PIN, ECanBitRate::B500k);
    communication.SetQueues(canBus.GetRxQueue(), canBus.GetTxQueue());

    updateScreenHandler.Begin();

    otaCommunication.SetPrepareLocalCallback(FreeMemoryForSelfOta);
}

/**
 * @brief Build the EEZ screens, starting on calibration if none is stored.
 */
static void InitializeScreens()
{
    ui_init();

    if (calibrationScreenHandler.CalibrationRequired())
    {
        Serial.println("No stored calibration, starting on the calibration screen");
        loadScreenNoAnim(SCREEN_ID_CALIBRATION_SCREEN);
    }
}

/**
 * @brief Startup, in the order the steps have to run.
 */
void setup()
{
    InitializeHardware();

    if (!AllocateDrawBuffers())
    {
        Serial.println("No draw buffers could be allocated, display disabled");
        while (true) { delay(1000); }
    }

    InitializeDisplay();
    InitializeStorage();
    InitializeBus();
    InitializeScreens();

    Serial.println("Setup done");
    DIAG_HEAP_REPORT("after setup");
}

/**
 * @brief Run the GUI at the rate it asks for, without blocking the loop.
 * @param now_ms Current millis(), sampled once by the caller.
 * @note The due-time compare is signed so a millis() wrap is a non-event, and
 *       the wait is clamped because lv_timer_handler() returns
 *       LV_NO_TIMER_READY when no timer is pending.
 */
static void ServiceLvgl(uint32_t now_ms)
{
    if ((int32_t)(now_ms - next_lv_handler_ms) >= 0)
    {
        uint32_t till_next = lv_timer_handler();
        if (till_next > LV_HANDLER_MAX_WAIT_MS)
        {
            till_next = LV_HANDLER_MAX_WAIT_MS;
        }
        next_lv_handler_ms = millis() + till_next;
    }
    else
    {
        vTaskDelay(1);
    }

    if (now_ms - last_ui_tick_ms >= UI_TICK_INTERVAL_MS)
    {
        last_ui_tick_ms = now_ms;
        ui_tick();
    }
}

/**
 * @brief Drain CAN, pump a self update, service the GUI.
 */
void loop()
{
    communication.CheckForMessage();

    otaCommunication.Handle();

    ServiceLvgl(millis());

    DIAG_HEAP_TICK();
}
