#ifndef CANMESSAGES_H
#define CANMESSAGES_H

struct CANAirRidePressure {
    float front;
    float back;
};

struct CANAirRideControl {
    bool frontUp;
    bool frontDown;
    bool backUp;
    bool backDown;
    bool ride;
    bool park;
};

struct CANSettingsMax {
    float frontMax;
    float backMax;
};

struct CANSettingsRide {
    float rideFront;
    float rideBack;
};

struct CANSettingsFrontFactor {
    float frontUpX;
    float frontDownX;
};

struct CANSettingsBackFactor {
    float backUpX;
    float backDownX;
};

struct CANSettingsPark {
    float parkDuration;
};

#endif // CANMESSAGES_H
