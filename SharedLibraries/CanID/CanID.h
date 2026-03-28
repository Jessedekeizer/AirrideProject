#ifndef CANID_H
#define CANID_H

#include "CanMessageIDs.h"

class CanId {
public:
    ECanNode src;
    ECanNode dst;
    ECanMsgType type;
    ECanFlags flags;

    CanId() : src(ECanNode::NODE_BROADCAST), dst(ECanNode::NODE_BROADCAST),
              type(ECanMsgType::CAN_NAN), flags(ECanFlags::FLAG_NONE) {
    }

    CanId(ECanNode src, ECanNode dst, ECanMsgType type, ECanFlags flags)
        : src(src), dst(dst), type(type), flags(flags) {
    }

    // Build the 29-bit integer for the CAN frame
    uint32_t ToRaw() const {
        return (static_cast<uint32_t>(static_cast<int8_t>(src) & 0x1F) << 24)
               | (static_cast<uint32_t>(static_cast<int8_t>(dst) & 0x1F) << 19)
               | (static_cast<uint32_t>(static_cast<int16_t>(type) & 0x7FFF) << 4)
               | static_cast<uint32_t>(static_cast<int8_t>(flags) & 0x3FF);
    }

    // Parse a raw 29-bit value back into fields
    bool FromRaw(uint32_t raw) {
        src = static_cast<ECanNode>((raw >> 24) & 0x1F);
        dst = static_cast<ECanNode>((raw >> 19) & 0x1F);
        type = static_cast<ECanMsgType>((raw >> 4) & 0x7FFF);
        flags = static_cast<ECanFlags>(raw & 0x0F);
        if (type == ECanMsgType::CAN_NAN) {
            return false;
        }
        return true;
    }

    bool IsForNode(ECanNode me) const {
        return dst == me || dst == ECanNode::NODE_BROADCAST;
    }

    bool HasFlag() const { return flags != ECanFlags::FLAG_NONE; }
};

#endif // CANID_H
