#include "SettingsStorage.h"
#include "Logger.h"

namespace {
constexpr uint32_t WRITER_STACK_BYTES = 6144;

constexpr UBaseType_t WRITER_PRIORITY = 1;

constexpr BaseType_t WRITER_CORE = 0;
}

/**
 * @brief Bind to the card.
 */
SettingsStorage::SettingsStorage(SdCardService &sdCardService) : sdCardService(sdCardService) {
}

/**
 * @brief Start the writer task and the card mutex.
 * @return true when the worker is running.
 * @note Optional. Without it every call is a blocking access on the caller
 *       thread, so a failed spawn degrades rather than breaks.
 */
bool SettingsStorage::Begin() {
    if (writerTask != nullptr) {
        return true;
    }

    cardMutex = xSemaphoreCreateMutex();
    writeQueue = xQueueCreate(1, sizeof(SettingsDevice));

    if (cardMutex == nullptr || writeQueue == nullptr) {
        LOG_WARN("Settings writer alloc failed, writes stay synchronous");
        return false;
    }

    const BaseType_t created = xTaskCreatePinnedToCore(
        WriterTask, "settings_wr", WRITER_STACK_BYTES, this,
        WRITER_PRIORITY, &writerTask, WRITER_CORE);

    if (created != pdPASS) {
        writerTask = nullptr;
        LOG_WARN("Settings writer task failed, writes stay synchronous");
        return false;
    }

    return true;
}

/**
 * @brief Worker on core 0 that performs queued writes.
 * @param param The SettingsStorage instance.
 * @warning Must never call into LVGL: that belongs to the UI task.
 */
void SettingsStorage::WriterTask(void *param) {
    SettingsStorage *self = static_cast<SettingsStorage *>(param);
    SettingsDevice snapshot;

    for (;;) {
        if (xQueueReceive(self->writeQueue, &snapshot, portMAX_DELAY) != pdTRUE) {
            continue;
        }

        if (self->LockCard()) {
            self->WriteSettingsLocked(snapshot);
            self->UnlockCard();
        }
    }
}

/**
 * @brief Queue a write and return immediately.
 * @param settings Snapshot to write.
 * @note The queue is one deep and a pending write is replaced, so holding
 *       Save down collapses to one write of the newest values.
 */
void SettingsStorage::WriteSettingsAsync(const SettingsDevice &settings) {
    if (writeQueue == nullptr) {
        WriteSettings(settings);
        return;
    }

    xQueueOverwrite(writeQueue, &settings);
}

/**
 * @brief Blocking write on the calling thread.
 * @param settings Values to write.
 */
void SettingsStorage::WriteSettings(const SettingsDevice &settings) {
    if (!LockCard()) {
        return;
    }
    WriteSettingsLocked(settings);
    UnlockCard();
}

/**
 * @brief Blocking read on the calling thread.
 * @param settings Filled from the card; left alone if it is not ready.
 */
void SettingsStorage::ReadSettings(SettingsDevice &settings) {
    if (!LockCard()) {
        return;
    }
    ReadSettingsLocked(settings);
    UnlockCard();
}

/**
 * @brief Take the card mutex. No-op when Begin() never ran.
 * @return true once held.
 */
bool SettingsStorage::LockCard() {
    if (cardMutex == nullptr) {
        return true;
    }
    return xSemaphoreTake(cardMutex, portMAX_DELAY) == pdTRUE;
}

/**
 * @brief Release the card mutex.
 */
void SettingsStorage::UnlockCard() {
    if (cardMutex != nullptr) {
        xSemaphoreGive(cardMutex);
    }
}

/**
 * @brief The write itself, with the mutex already held.
 * @param settings Values to write.
 */
void SettingsStorage::WriteSettingsLocked(const SettingsDevice &settings) {
    if (!sdCardService.IsReady())
        return;

    fs::FS &fs = sdCardService.GetFileSystem();

    if (!fs.remove("/settings.bin")) {
        LOG_DEBUG("Cannot remove file");
    }

    File file = fs.open("/settings.bin", FILE_WRITE);
    if (!file) {
        LOG_DEBUG("Cannot open file to write");
        return;
    }

    size_t bytesWritten = file.write((const uint8_t *)&settings, sizeof(settings));

    if (bytesWritten != sizeof(settings)) {
        LOG_DEBUG("Failed to write settings");
    }
    else {
        LOG_DEBUG("Settings saved successfully");
    }

    file.close();
}

/**
 * @brief The read itself, with the mutex already held.
 * @param settings Filled from the card.
 */
void SettingsStorage::ReadSettingsLocked(SettingsDevice &settings) {
    if (!sdCardService.IsReady())
        return;

    File file = sdCardService.GetFileSystem().open("/settings.bin", FILE_READ);
    if (!file) {
        LOG_DEBUG("Cannot open file to read");
        return;
    }

    size_t bytesRead = file.read((uint8_t *)&settings, sizeof(settings));

    if (bytesRead != sizeof(settings)) {
        LOG_DEBUG("Failed to read settings");
    }
    else {
        LOG_DEBUG("Settings loaded successfully");
    }

    file.close();
}
