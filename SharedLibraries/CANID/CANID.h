#ifndef CANID_H
#define CANID_H

#include "CANMessageIDs.h"

class CanId {
public:
    CanNode src;
    CanNode dst;
    CanMsgType type;
    CanFlags flags;

    CanId(CanNode src, CanNode dst, CanMsgType type, CanFlags flags)
        : src(src), dst(dst), type(type), flags(flags) {
    }

    // Build the 29-bit integer for the CAN frame
    uint32_t toRaw() const {
        return ((uint32_t)(src & 0x1F) << 24)
               | ((uint32_t)(dst & 0x1F) << 19)
               | ((uint32_t)(type & 0x1FF) << 10)
               | ((uint32_t)(flags & 0x3FF));
    }

    // Parse a raw 29-bit value back into fields
    static CanId fromRaw(uint32_t raw) {
        return CanId(
            (CanNode)((raw >> 24) & 0x1F),
            (CanNode)((raw >> 19) & 0x1F),
            (CanMsgType)((raw >> 10) & 0x1FF),
            (CanFlags)(raw & 0x3FF)
        );
    }

    bool isForNode(CanNode me) const {
        return dst == me || dst == NODE_BROADCAST;
    }

    bool hasFlag(CanFlags f) const { return (flags & f) != 0; }
};

#endif // CANID_H
