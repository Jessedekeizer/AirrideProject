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
