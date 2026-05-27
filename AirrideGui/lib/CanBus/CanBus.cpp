#include "CanBus.h"
#include "Logger.h"

#define TIME_BETWEEN_TX_MESSAGES_IN_MS 5

void CanBus::Setup(int canTx, int canRx, ECanBitRate bitRate)
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        static_cast<gpio_num_t>(canTx),
        static_cast<gpio_num_t>(canRx),
        TWAI_MODE_NORMAL);

    twai_timing_config_t t_config = TWAI_TIMING_CONFIG_125KBITS();

    switch (bitRate)
    {
    case ECanBitRate::B125K:
        t_config = TWAI_TIMING_CONFIG_125KBITS();
        break;
    case ECanBitRate::B250k:
        t_config = TWAI_TIMING_CONFIG_250KBITS();
        break;
    case ECanBitRate::B500k:
        t_config = TWAI_TIMING_CONFIG_500KBITS();
        break;
    }

    twai_filter_config_t f_config = TWAI_FILTER_CONFIG_ACCEPT_ALL();

    if (twai_driver_install(&g_config, &t_config, &f_config) != ESP_OK)
    {
        LOG_ERROR("Failed to install TWAI driver");
        return;
    }

    if (twai_start() != ESP_OK)
    {
        LOG_ERROR("Failed to start TWAI");
        return;
    }

    rxQueue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CanMessage));
    txQueue = xQueueCreate(CAN_TX_QUEUE_SIZE, sizeof(CanMessage));

    xTaskCreatePinnedToCore(RxTask, "CAN_RX", 4096, this, 5, nullptr, 0);
    xTaskCreatePinnedToCore(TxTask, "CAN_TX", 4096, this, 5, nullptr, 0);

    LOG_INFO("CAN Started at", static_cast<int>(bitRate));
    canReady = true;
}

void CanBus::RxTask(void *arg)
{
    CanBus *self = static_cast<CanBus *>(arg);
    while (true)
    {
        twai_message_t rx_msg;
        if (twai_receive(&rx_msg, portMAX_DELAY) == ESP_OK)
        {
            CanMessage message{0};
            message.id = rx_msg.identifier;
            message.dlc = rx_msg.data_length_code;
            memcpy(message.data, rx_msg.data, rx_msg.data_length_code);
            if (xQueueSend(self->rxQueue, &message, 0) != pdTRUE)
            {
                LOG_DEBUG("RX: Queue full");
            }
        }
    }
}

void CanBus::TxTask(void *arg)
{
    CanBus *self = static_cast<CanBus *>(arg);
    CanMessage message;
    while (true)
    {
        if (xQueueReceive(self->txQueue, &message, portMAX_DELAY) == pdTRUE)
        {
            twai_message_t tx_msg = {};
            tx_msg.identifier = message.id;
            tx_msg.extd = 1;
            tx_msg.rtr = 0;
            tx_msg.data_length_code = message.dlc;
            for (int i = 0; i < 8; i++)
            {
                tx_msg.data[i] = message.data[i];
            }
            if (twai_transmit(&tx_msg, pdMS_TO_TICKS(100)) != ESP_OK)
            {
                LOG_ERROR("TX: Failed");
            }
            vTaskDelay(pdMS_TO_TICKS(TIME_BETWEEN_TX_MESSAGES_IN_MS));
        }
    }
}
