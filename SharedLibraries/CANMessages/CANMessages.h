#ifndef CANMESSAGES_H
#define CANMESSAGES_H
#include <cstdint>

#include "Logger.h"

#pragma pack(push,1)
struct CANAirRidePressure {
    float front;
    float back;
};
#pragma pack(pop)

#pragma pack(push,1)
struct CANAirRideControl {
    bool frontUp;
    bool frontDown;
    bool backUp;
    bool backDown;
    bool ride;
    bool park;
};
#pragma pack(pop)

#pragma pack(push,1)
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
#pragma pack(pop)

#pragma pack(push,1)
struct CANLogAirRide {
    bool front;
    float startPressure;
    float endPressure;
    float startTankPressure;
    uint16_t time;
    bool direction;
    bool togetherMove;
};
#pragma pack(pop)

template<typename T>
bool decodeCANMessage(const uint8_t *data, std::size_t length, T &messageStruct) {
    if (length == sizeof(T)) {
        memcpy(&messageStruct, data, sizeof(T));
        return true;
    }
    LOG_DEBUG("copy failed length: ", length, " struct size: ", sizeof(T));
    return false;
}

#endif // CANMESSAGES_H
