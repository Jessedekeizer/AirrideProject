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
    if (canTx == CAN1TX && canRx == CAN1RX) {
        SetCan1Pins();
    }

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


void CanBus::SetCan1Pins() {
    //Clear BOWI
    R_PMISC->PWPR = 0x00;
    //Unlock PSFWE for write
    R_PMISC->PWPR = 0x40;

    //Release D4/D5 (P103/P102) back to GPIO
    R_PFS->PORT[1].PIN[2].PmnPFS = 0;
    R_PFS->PORT[1].PIN[3].PmnPFS = 0;

    //P109 = D11 = CAN TX — output, PDR=1
    R_PFS->PORT[1].PIN[9].PmnPFS = (0x10UL << 24) | (1UL << 16) | (1UL << 2);

    //P110 = D12 = CAN RX — input, no PDR
    R_PFS->PORT[1].PIN[10].PmnPFS = (0x10UL << 24) | (1UL << 16);

    //Clear BOWI
    R_PMISC->PWPR = 0x00;
    //lock PSFWE
    R_PMISC->PWPR = 0x80;
}
