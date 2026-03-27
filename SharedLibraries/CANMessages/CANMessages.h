#ifndef CANMESSAGES_H
#define CANMESSAGES_H
#include <cstdint>

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

struct CANSettingsAirRide {
    float frontMax;
    float backMax;
    float rideFront;
    float rideBack;
    float frontUpX;
    float frontDownX;
    float backUpX;
    float backDownX;
    float parkDuration;
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

template<typename T>
T decodeCANMessage(const uint8_t *data, std::size_t length) {
    T messageStruct{};
    if (length == sizeof(T)) {
        memcpy(&messageStruct, data, sizeof(T));
    }
    return messageStruct;
}

#endif // CANMESSAGES_H
