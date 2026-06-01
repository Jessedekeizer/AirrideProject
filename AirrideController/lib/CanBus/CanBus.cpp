#include "CanBus.h"
#include <Arduino_CAN.h>
#include "Logger.h"

#define TIME_BETWEEN_TX_MESSAGES_IN_MS 5

void CanBus::Setup(int canTx, int canRx, ECanBitRate bitRate) {
    CanBitRate canBitRate = CanBitRate::BR_125k;
    switch (bitRate) {
        case ECanBitRate::B125K:
            canBitRate = CanBitRate::BR_125k;
            break;
        case ECanBitRate::B250k:
            canBitRate = CanBitRate::BR_250k;
            break;
        case ECanBitRate::B500k:
            canBitRate = CanBitRate::BR_500k;
            break;
    }
    if (!CAN.begin(canBitRate)) {
        LOG_ERROR("CAN.begin(...) failed.");
        return;
    }

#if(ARDUINO_MINIMA)
    if (canTx == CAN1TX && canRx == CAN1RX) {
        SetCan1Pins();
    }
#endif


    rxQueue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CanMessage));
    txQueue = xQueueCreate(CAN_TX_QUEUE_SIZE, sizeof(CanMessage));

    xTaskCreate(RxTask, "CAN_RX", 128, this, 4, nullptr);
    xTaskCreate(TxTask, "CAN_TX", 128, this, 4, nullptr);

    LOG_INFO("CAN initialized");
    canReady = true;
}

void CanBus::RxTask(void *arg) {
    CanBus *self = static_cast<CanBus *>(arg);
    while (true) {
        if (CAN.available()) {
            CanMsg const msg = CAN.read();
            CanMessage message{};
            message.id = msg.id;
            message.dlc = msg.data_length;
            memcpy(message.data, msg.data, msg.data_length);
            if (xQueueSend(self->rxQueue, &message, 0) != pdTRUE) {
                LOG_ERROR("RX: Queue full");
            }
        } else {
            vTaskDelay(pdMS_TO_TICKS(1));
        }
    }
}

void CanBus::TxTask(void *arg) {
    CanBus *self = static_cast<CanBus *>(arg);
    CanMessage message{};
    while (true) {
        if (xQueueReceive(self->txQueue, &message, portMAX_DELAY) == pdTRUE) {
            CanMsg msg;
            msg.id = CanExtendedId(message.id);
            msg.data_length = message.dlc;
            memcpy(msg.data, message.data, message.dlc);
            if (int const rc = CAN.write(msg); rc < 0) {
                LOG_ERROR("TX: Failed with code:", rc);
            }
            vTaskDelay(pdMS_TO_TICKS(TIME_BETWEEN_TX_MESSAGES_IN_MS));
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
