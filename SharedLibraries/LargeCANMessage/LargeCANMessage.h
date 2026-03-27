#ifndef LARGECANMESSAGE_H
#define LARGECANMESSAGE_H
#include <cstdint>
#include <vector>
#include "CANID.h"

struct LargeCanMessage {
    CanId id;
    std::vector<uint8_t> data;
    uint8_t length;
};

#endif // LARGECANMESSAGE_H
