#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H
#include "SdCardService.h"
#include "SettingsDevice.h"

#include <freertos/FreeRTOS.h>
#include <freertos/queue.h>
#include <freertos/semphr.h>
#include <freertos/task.h>

class SettingsStorage {
public:
    SettingsStorage(SdCardService &sdCardService);

    bool Begin();

    void WriteSettingsAsync(const SettingsDevice &settings);

    void WriteSettings(const SettingsDevice &settings);

    void ReadSettings(SettingsDevice &settings);

private:
    static void WriterTask(void *param);

    void WriteSettingsLocked(const SettingsDevice &settings);
    void ReadSettingsLocked(SettingsDevice &settings);

    bool LockCard();
    void UnlockCard();

    SdCardService &sdCardService;
    QueueHandle_t writeQueue = nullptr;
    SemaphoreHandle_t cardMutex = nullptr;
    TaskHandle_t writerTask = nullptr;
};

#endif
