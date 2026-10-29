#ifndef CANMESSAGEIDS_H
#define CANMESSAGEIDS_H
#include <cstdint>

enum class ECanNode : uint8_t {
    NODE_BROADCAST = 0,
    NODE_AIRRIDE_CONTROLLER = 1,
    NODE_AIRRIDE_GUI = 2,
    first = NODE_BROADCAST,
    last = NODE_AIRRIDE_GUI,
    UNKNOWN
};

enum class ECanMsgType : uint16_t {
    //Control start from 0x0
    CAN_AIRRIDE_CONTROL = 0x001,

    //Telemetry starts from 0x1
    CAN_AIRRIDE_PRESSURE = 0x101,

    //Settings for airride start from 0x6
    CAN_AIRRIDE_SETTINGS = 0x601,

    //Logs from 0x7
    CAN_AIRRIDE_LOG = 0x701,

    //OTA from 0x8
    CAN_AIRRIDE_OTA = 0x801,
    CAN_AIRRIDE_OTA_STATUS = 0x802,

    //Ack from 0x9
    CAN_AIRRIDE_ACK = 0x901,
    //MAX 0x7FFF

    first = CAN_AIRRIDE_CONTROL,
    last = CAN_AIRRIDE_ACK,
    UNKNOWN
};

// Bits 0-1: fragment state of a large message (mutually exclusive).
// Bit 2: ack-required, OR-able onto any fragment state (set only on the
// terminal frame of a send - the only frame for a small message, or the
// FLAG_LAST fragment for a large one - so exactly one ack fires per struct).
enum class ECanFlags : uint8_t {
    FLAG_NONE = 0,
    FLAG_LARGE_MESSAGE = 1,
    FLAG_FIRST = 2,
    FLAG_LAST = 3,
    FLAG_ACK_REQUIRED = 4,

    first = FLAG_NONE,
    last = FLAG_ACK_REQUIRED,
    UNKNOWN = 255
};

enum class ECanAckStatus : uint8_t {
    STATUS_OK = 0,
    STATUS_ERROR = 1,

    first = STATUS_OK,
    last = STATUS_ERROR,
    UNKNOWN
};

#endif // CANMESSAGEIDS_H
