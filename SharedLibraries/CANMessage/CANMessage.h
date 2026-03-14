#ifndef CANMESSAGE_H
#define CANMESSAGE_H
#include <cstdint>

#pragma pack(push,1)
struct CANMessage {
    uint16_t id;
    uint8_t dlc;
    uint8_t data[8];
};
#pragma pack(pop)

template<typename T>
T decodeCANMessage(const CANMessage &canMsg) {
    T messageStruct;
    uint8_t copyLength = sizeof(T) < canMsg.dlc ? sizeof(T) : canMsg.dlc;

    memset(&messageStruct, 0, sizeof(T));
    memcpy(&messageStruct, canMsg.data, copyLength);

    return messageStruct;
}

#endif // CANMESSAGE_H
