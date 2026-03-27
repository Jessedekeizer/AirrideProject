#ifndef CANID_H
#define CANID_H

#include "CANMessageIDs.h"

class CanId {
public:
    CanNode src;
    CanNode dst;
    CanMsgType type;
    CanFlags flags;

    CanId() : src(CanNode::NODE_NAN), dst(CanNode::NODE_NAN),
              type(CanMsgType::CAN_NAN), flags(CanFlags::FLAG_NAN) {
    }

    CanId(CanNode src, CanNode dst, CanMsgType type, CanFlags flags)
        : src(src), dst(dst), type(type), flags(flags) {
    }

    // Build the 29-bit integer for the CAN frame
    uint32_t toRaw() const {
        return (static_cast<uint32_t>(static_cast<int8_t>(src) & 0x1F) << 24)
               | (static_cast<uint32_t>(static_cast<int8_t>(dst) & 0x1F) << 19)
               | (static_cast<uint32_t>(static_cast<int16_t>(type) & 0x1FF) << 10)
               | static_cast<uint32_t>(static_cast<int8_t>(flags) & 0x3FF);
    }

    // Parse a raw 29-bit value back into fields
    bool fromRaw(uint32_t raw) {
        src = static_cast<CanNode>((raw >> 24) & 0x1F);
        dst = static_cast<CanNode>((raw >> 19) & 0x1F);
        type = static_cast<CanMsgType>((raw >> 10) & 0x1FF);
        flags = static_cast<CanFlags>(raw & 0x3FF);
        if (src == CanNode::NODE_NAN || dst == CanNode::NODE_NAN || type == CanMsgType::CAN_NAN ||
            flags == CanFlags::FLAG_NONE) {
            return false;
        }
        return true;
    }

    bool isForNode(CanNode me) const {
        return dst == me || dst == CanNode::NODE_BROADCAST;
    }

    bool hasFlag() const { return flags != CanFlags::FLAG_NONE; }
};

#endif // CANID_H
