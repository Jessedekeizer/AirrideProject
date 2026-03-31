#include "SettingsStorage.h"
#include "Logger.h"

SettingsStorage::SettingsStorage(SdCardService &sdCardService) : sdCardService(sdCardService) {
}

void SettingsStorage::WriteSettings(SettingsDevice &settings) {
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

    // Write the entire struct in one go
    size_t bytesWritten = file.write((const uint8_t *)&settings, sizeof(settings));

    if (bytesWritten != sizeof(settings)) {
        LOG_DEBUG("Failed to write settings");
    }
    else {
        LOG_DEBUG("Settings saved successfully");
    }

    file.close();
}

void SettingsStorage::ReadSettings(SettingsDevice &settings) {
    if (!sdCardService.IsReady())
        return;

    File file = sdCardService.GetFileSystem().open("/settings.bin", FILE_READ);
    if (!file) {
        LOG_DEBUG("Cannot open file to read");
        return;
    }

    // Read the entire struct in one go
    size_t bytesRead = file.read((uint8_t *)&settings, sizeof(settings));

    if (bytesRead != sizeof(settings)) {
        LOG_DEBUG("Failed to read settings");
    }
    else {
        LOG_DEBUG("Settings loaded successfully");
    }

    file.close();
}
