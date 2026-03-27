#ifndef CANMESSAGE_H
#define CANMESSAGE_H
#include <cstdint>

#pragma pack(push,1)
struct CanMessage {
    uint32_t id;
    uint8_t dlc;
    uint8_t data[8];
};
#pragma pack(pop)

#endif // CANMESSAGE_H
