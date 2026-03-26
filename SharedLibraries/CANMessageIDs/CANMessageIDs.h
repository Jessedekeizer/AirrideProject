#ifndef CANMESSAGEIDS_H
#define CANMESSAGEIDS_H
#include <cstdint>

typedef enum : uint16_t {
    CANSettingsRide = 0x602,
    CANSettingsFrontFactor = 0x603,
    CANSettingsBackFactor = 0x604,
    CANSettingsPark = 0x605
} CAN_ID;

typedef enum : uint8_t {
    NODE_BROADCAST = 0x00,
    NODE_R4 = 0x01,
    NODE_ESP32 = 0x02,
} CanNode;

typedef enum : uint16_t {
    //Control start from 0x0
    CANAirRideControl = 0x001,

    //Telemetry starts from 0x1
    CANAirRidePressure = 0x101,

    //Settings for airride start from 0x6
    AirRideSettings = 0x601,
} CanMsgType;

typedef enum : uint16_t {
    FLAG_NONE = 0x000,
    FLAG_LARGE_MESSAGE = 0x001,
    FLAG_FIRST = 0x002,
    FLAG_LAST = 0x003
} CanFlags;

#endif // CANMESSAGEIDS_H
