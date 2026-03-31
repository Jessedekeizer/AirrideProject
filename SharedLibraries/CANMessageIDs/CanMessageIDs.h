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
    //MAX 0x7FFF

    first = CAN_AIRRIDE_CONTROL,
    last = CAN_AIRRIDE_LOG,
    UNKNOWN
};

enum class ECanFlags : uint8_t {
    FLAG_NONE = 0,
    FLAG_LARGE_MESSAGE = 1,
    FLAG_FIRST = 2,
    FLAG_LAST = 3,

    first = FLAG_NONE,
    last = FLAG_LAST,
    UNKNOWN
};

#endif // CANMESSAGEIDS_H
