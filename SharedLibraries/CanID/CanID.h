#ifndef CANID_H
#define CANID_H

#include "CanMessageIds.h"

class CanId {
public:
    ECanNode src;
    ECanNode dst;
    ECanMsgType type;
    ECanFlags flags;

    CanId();

    CanId(ECanNode src, ECanNode dst, ECanMsgType type, ECanFlags flags);

    uint32_t ToRaw() const;

    // Parse a raw 29-bit value back into fields
    bool FromRaw(uint32_t raw);

    bool IsForNode(ECanNode me) const;

    bool HasFlag() const;

private:
    bool FromRawValid();
};
#endif // CANID_H
