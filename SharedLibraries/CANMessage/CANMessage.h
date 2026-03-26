#ifndef CANMESSAGE_H
#define CANMESSAGE_H
#include <cstdint>

#pragma pack(push,1)
struct CANMessage {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};
#pragma pack(pop)

template<typename T>
T decodeCANMessage(uint8_t *data, std::size_t length) {
    T messageStruct{};
    if (length == sizeof(T)) {
        memcpy(&messageStruct, data, sizeof(T));
    }
    return messageStruct;
}

#endif // CANMESSAGE_H
