#ifndef CANMESSAGEIDS_H
#define CANMESSAGEIDS_H
#include <cstdint>

enum class CAN_ID : uint16_t {
    //Control start from 0x0
    CANAirRideControl = 0x001,

    //Telemetry starts from 0x1
    CANAirRidePressure = 0x101,

    //Settings for airride start from 0x6
    CANSettingsMax = 0x601,
    CANSettingsRide = 0x602,
    CANSettingsFrontFactor = 0x603,
    CANSettingsBackFactor = 0x604,
    CANSettingsPark = 0x605
};

#endif // CANMESSAGEIDS_H
