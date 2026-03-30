#include "CanBus.h"
#include <Arduino_CAN.h>

#include "Logger.h"

void CanBus::Setup(int canTx, int canRx, ECanBitRate bitRate) {
    CanBitRate canBitRate;
    switch (bitRate) {
        case ECanBitRate::B125K: {
            canBitRate = CanBitRate::BR_125k;
            break;
        }
        case ECanBitRate::B250k: {
            canBitRate = CanBitRate::BR_250k;
            break;
        }
        case ECanBitRate::B500k: {
            canBitRate = CanBitRate::BR_500k;
            break;
        }
    }
    if (!CAN.begin(canBitRate)) {
        LOG_ERROR("CAN.begin(...) failed.");
        canReady = false;
    }
    SetDifferentPins();

    LOG_DEBUG("CAN initialized");
    canReady = true;
}

void CanBus::SendMessage(CanMessage &message) {
    if (!canReady) {
        return;
    }
    CanMsg msg;
    msg.id = CanExtendedId(message.id);
    msg.data_length = message.dlc;
    memcpy(msg.data, message.data, message.dlc);
    if (int const rc = CAN.write(msg); rc < 0) {
        LOG_ERROR("Message sent failed with code:", rc);
    }
}

bool CanBus::ReceiveAvailable() {
    return CAN.available();
}

void CanBus::Receive() {
    if (!canReady) {
        return;
    }
    while (CAN.available()) {
        CanMsg const msg = CAN.read();
        LOG_DEBUG("Received message:", msg);
        CanMessage message;
        message.id = msg.id;
        message.dlc = msg.data_length;
        memcpy(message.data, msg.data, msg.data_length);
        if (!canQueue.enqueue(message)) {
            LOG_ERROR("CanQueue is full");
        }
    }
}

void CanBus::SetDifferentPins() {
    R_PMISC->PWPR = 0x00;
    R_PMISC->PWPR = 0x40;

    // Release D4/D5 (P206/P205) back to GPIO
    R_PFS->PORT[2].PIN[6].PmnPFS = 0;
    R_PFS->PORT[2].PIN[5].PmnPFS = 0;

    // P109 = D12 = CRXD0 (CAN RX) — input, PSEL=0x10, PMR=1
    R_PFS->PORT[1].PIN[9].PmnPFS = (0x10UL << 24) | (1UL << 16);

    // P110 = D13 = CTXD0 (CAN TX) — output, PSEL=0x10, PMR=1, PDR=1
    R_PFS->PORT[1].PIN[10].PmnPFS = (0x10UL << 24) | (1UL << 16) | (1UL << 2);

    R_PMISC->PWPR = 0x00;
    R_PMISC->PWPR = 0x80;
}
