#ifndef SETTINGSSTORAGE_H
#define SETTINGSSTORAGE_H
#include "SdCardService.h"
#include "SettingsDevice.h"

class SettingsStorage {
public:
    SettingsStorage(SdCardService &sdCardService);
    void WriteSettings(SettingsDevice &settings);
    void ReadSettings(SettingsDevice &settings);

private:
    SdCardService &sdCardService;
};

#endif //SETTINGSSTORAGE_H
