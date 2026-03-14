#ifndef SETTINGSDEVICE_H
#define SETTINGSDEVICE_H

struct SettingsDevice {
    float frontMax = 0.0;
    float backMax = 0.0;
    float rideFront = 0.0;
    float rideBack = 0.0;
    float frontUpX = 0.0;
    float frontDownX = 0.0;
    float backUpX = 0.0;
    float backDownX = 0.0;
    bool autoRide = false;
    float autoRideSec = 0.0;
    bool autoPark = false;
    float autoParkSec = 0.0;
    bool logging = false;
    bool machineLearning = false;
    bool bluetooth = false;
    float parkDuration = 0.0;
    int xmin = 0;
    int xmax = 320;
    int ymin = 0;
    int ymax = 240;
    bool calibrationSet = false;

    // Helper method to ensure values don't go below 0
    void adjustValue(float &value, float delta) {
        value += delta;
        if (value < 0.0)
            value = 0.0;
    }
};

#endif // SETTINGS_H
