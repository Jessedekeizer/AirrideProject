#include "CanID.h"
#include "Logger.h"

CanId::CanId() : src(ECanNode::UNKNOWN), dst(ECanNode::UNKNOWN),
                 type(ECanMsgType::UNKNOWN), flags(ECanFlags::UNKNOWN) {
}

CanId::CanId(ECanNode src, ECanNode dst, ECanMsgType type, ECanFlags flags)
    : src(src), dst(dst), type(type), flags(flags) {
}

uint32_t CanId::ToRaw() const {
    return (static_cast<uint32_t>(static_cast<int8_t>(src) & 0x1F) << 24)
           | (static_cast<uint32_t>(static_cast<int8_t>(dst) & 0x1F) << 19)
           | (static_cast<uint32_t>(static_cast<int16_t>(type) & 0x7FFF) << 4)
           | static_cast<uint32_t>(static_cast<int8_t>(flags) & 0x3FF);
}

bool CanId::FromRaw(uint32_t raw) {
    src = static_cast<ECanNode>((raw >> 24) & 0x1F);
    dst = static_cast<ECanNode>((raw >> 19) & 0x1F);
    type = static_cast<ECanMsgType>((raw >> 4) & 0x7FFF);
    flags = static_cast<ECanFlags>(raw & 0x0F);
    return FromRawValid();
}

bool CanId::IsForNode(ECanNode me) const {
    return dst == me || dst == ECanNode::NODE_BROADCAST;
}

bool CanId::HasFlag() const {
    return flags != ECanFlags::FLAG_NONE && flags != ECanFlags::UNKNOWN;
}

bool CanId::FromRawValid() {
    if (src != ECanNode::UNKNOWN && dst != ECanNode::UNKNOWN && type != ECanMsgType::UNKNOWN && flags != ECanFlags::UNKNOWN) {
        return true;
    }
    LOG_ERROR("CanID parse failed: src:", static_cast<int8_t>(src), "dst:",
                  static_cast<int8_t>(dst), "type:", static_cast<int16_t>(type), "flags:",
                  static_cast<int8_t>(flags));
    return false;
}

