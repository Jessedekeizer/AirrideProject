#include "CanBus.h"
#include "Logger.h"

#define TIME_BETWEEN_TX_MESSAGES_IN_MS 5

#define DROP_LOG_INTERVAL_MS 1000

/**
 * @brief Bring the bus up and start the RX and TX tasks.
 * @param canTx   Transmit GPIO.
 * @param canRx   Receive GPIO.
 * @param bitRate Bus speed.
 * @note Blocks until the driver is installed, so the queues are valid on
 *       return. The install runs on CAN_CORE rather than inline because
 *       esp_intr_alloc() binds the interrupt to whichever core calls it.
 */
void CanBus::Setup(int canTx, int canRx, ECanBitRate bitRate)
{
    txPin = canTx;
    rxPin = canRx;
    this->bitRate = bitRate;

    setupDone = xSemaphoreCreateBinary();
    if (setupDone == nullptr)
    {
        LOG_ERROR("Failed to create the CAN setup semaphore");
        return;
    }

    if (xTaskCreatePinnedToCore(InstallTask, "CAN_INIT", 4096, this, 5, nullptr, CAN_CORE) != pdPASS)
    {
        LOG_ERROR("Failed to create the CAN install task");
        vSemaphoreDelete(setupDone);
        setupDone = nullptr;
        return;
    }

    xSemaphoreTake(setupDone, portMAX_DELAY);
    vSemaphoreDelete(setupDone);
    setupDone = nullptr;

    if (!canReady)
    {
        return;
    }

    rxQueue = xQueueCreate(CAN_RX_QUEUE_SIZE, sizeof(CanMessage));
    txQueue = xQueueCreate(CAN_TX_QUEUE_SIZE, sizeof(CanMessage));

    xTaskCreatePinnedToCore(RxTask, "CAN_RX", 4096, this, 5, &rxTask, CAN_CORE);
    xTaskCreatePinnedToCore(TxTask, "CAN_TX", 4096, this, 5, &txTask, CAN_CORE);

    LOG_INFO("CAN Started at", static_cast<int>(bitRate), "on core", CAN_CORE);
}

/**
 * @brief Shut the bus down and free the driver, tasks and queues.
 * @warning The queues are destroyed. Anything holding a copy of a handle must
 *          drop it first - Communication::SetQueues() has copies.
 * @note There is no matching start; the display reboots instead.
 */
void CanBus::Stop()
{
    if (!canReady)
    {
        return;
    }
    canReady = false;

    if (rxTask != nullptr)
    {
        vTaskDelete(rxTask);
        rxTask = nullptr;
    }
    if (txTask != nullptr)
    {
        vTaskDelete(txTask);
        txTask = nullptr;
    }

    twai_stop();
    twai_driver_uninstall();

    if (rxQueue != nullptr)
    {
        vQueueDelete(rxQueue);
        rxQueue = nullptr;
    }
    if (txQueue != nullptr)
    {
        vQueueDelete(txQueue);
        txQueue = nullptr;
    }

    LOG_INFO("CAN stopped, memory released");
}

/**
 * @brief One-shot task that installs the driver on CAN_CORE, then exits.
 * @param arg The CanBus instance.
 */
void CanBus::InstallTask(void *arg)
{
    CanBus *self = static_cast<CanBus *>(arg);
    self->canReady = self->InstallDriver();
    xSemaphoreGive(self->setupDone);
    vTaskDelete(nullptr);
}

/**
 * @brief Install and start the TWAI driver.
 * @return true on success.
 */
bool CanBus::InstallDriver()
{
    twai_general_config_t g_config = TWAI_GENERAL_CONFIG_DEFAULT(
        static_cast<gpio_num_t>(txPin),
        static_cast<gpio_num_t>(rxPin),
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
        return false;
    }

    if (twai_start() != ESP_OK)
    {
        LOG_ERROR("Failed to start TWAI");
        twai_driver_uninstall();
        return false;
    }

    return true;
}

/**
 * @brief Move received frames from the driver into the RX queue.
 * @param arg The CanBus instance.
 * @note Drops are counted and reported at most once a second: an overflow is a
 *       burst, not a single frame.
 */
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
                self->droppedFrames++;
                const uint32_t now = millis();
                if (now - self->lastDropLogMs >= DROP_LOG_INTERVAL_MS)
                {
                    LOG_DEBUG("RX: Queue full, frames dropped:", self->droppedFrames);
                    self->lastDropLogMs = now;
                    self->droppedFrames = 0;
                }
            }
        }
    }
}

/**
 * @brief Send queued frames, spaced by TIME_BETWEEN_TX_MESSAGES_IN_MS.
 * @param arg The CanBus instance.
 */
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
