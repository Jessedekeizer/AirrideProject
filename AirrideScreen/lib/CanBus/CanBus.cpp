#include "CanBus.h"

#include "BaseScreen.h"
#include "driver/twai.h"
#include "Logger.h"

void CanBus::SendMessage(CanMessage &message)
{
    if (!canReady)
    {
        LOG_ERROR("CAN not ready");
        return;
    }
    twai_message_t tx_msg = {};
    tx_msg.identifier = message.id;
    tx_msg.extd = 1; // extended frame
    tx_msg.rtr = 0;
    tx_msg.data_length_code = message.dlc;

    for (int i = 0; i < 8; i++)
    {
        tx_msg.data[i] = message.data[i];
    }

    if (twai_transmit(&tx_msg, pdMS_TO_TICKS(100)) == ESP_OK)
    {
        LOG_DEBUG("TX: Message sent");
    }
    else
    {
        LOG_ERROR("TX: Failed");
    }
}

bool CanBus::ReceiveAvailable()
{
    if (!canReady)
    {
        return false;
    }
    twai_status_info_t status;
    twai_get_status_info(&status);
    if (status.msgs_to_rx > 0)
    {
        return true;
    }
    return false;
}

// Adjust these pins to your wiring
#define CAN_TX GPIO_NUM_22
#define CAN_RX GPIO_NUM_27

void CanBus::Setup(int canTx, int canRx, ECanBitRate bitRate)
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        static_cast<gpio_num_t>(canTx),
        static_cast<gpio_num_t>(canRx),
        TWAI_MODE_NORMAL);

    twai_timing_config_t t_config;

    switch (bitRate)
    {
    case ECanBitRate::B125K:
    {
        t_config = TWAI_TIMING_CONFIG_125KBITS();
        break;
    }
    case ECanBitRate::B250k:
    {
        t_config = TWAI_TIMING_CONFIG_250KBITS();
        break;
    }
    case ECanBitRate::B500k:
    {
        t_config = TWAI_TIMING_CONFIG_500KBITS();
        break;
    }
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

    LOG_INFO("CAN Started at", static_cast<int>(bitRate));
    canReady = true;
}

void CanBus::Receive()
{
    if (!canReady)
    {
        LOG_ERROR("CAN not ready");
        return;
    }
    twai_status_info_t status;
    twai_get_status_info(&status);

    while (status.msgs_to_rx > 0)
    {
        twai_message_t rx_msg;
        CanMessage message{0};

        if (twai_receive(&rx_msg, 0) == ESP_OK)
        {
            message.id = rx_msg.identifier;
            message.dlc = rx_msg.data_length_code;
            memcpy(message.data, rx_msg.data, rx_msg.data_length_code);
            if (!canQueue.enqueue(message))
            {
                LOG_DEBUG("Receive: queue full");
            }
        }
        twai_get_status_info(&status);
    }
}
