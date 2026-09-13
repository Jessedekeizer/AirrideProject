#include "SelfOTA.h"

#include <ArduinoOTA.h>
#include <WiFi.h>

#include "Logger.h"

#ifndef SELF_OTA_SSID
#define SELF_OTA_SSID "Airride-OTA"
#endif
#ifndef SELF_OTA_PASS
#define SELF_OTA_PASS "change-me-ota"
#endif

/**
 * @brief Set the callback progress is reported through.
 * @param cb Receives the same status struct a remote node would send.
 */
void SelfOTA::Init(StatusCallback cb) {
    onStatus = cb;
}

/**
 * @brief Bring up the access point and wait for an image to be pushed.
 * @warning Allocates tens of KB and blocks for a second or two.
 */
void SelfOTA::Start() {
    currentStatus = {};
    currentStatus.type = EOTAStatusType::UPDATE;

    WiFi.mode(WIFI_AP);
    WiFi.softAP(SELF_OTA_SSID, SELF_OTA_PASS);

    ArduinoOTA.setHostname("airride-gui");
    ArduinoOTA.onStart([this]() {
        currentStatus.phase = EOTAUpdatePhase::FLASHING;
        Notify();
    });
    ArduinoOTA.onEnd([this]() {
        currentStatus.phase = EOTAUpdatePhase::COMPLETE;
        currentStatus.progress = 100;
        Notify();
    });
    ArduinoOTA.onProgress([this](unsigned int progress, unsigned int total) {
        currentStatus.progress = (uint8_t) (progress * 100 / total);
        Notify();
    });
    ArduinoOTA.onError([this](ota_error_t err) {
        LOG_ERROR("SelfOTA error: ", err);
        currentStatus.phase = EOTAUpdatePhase::ERROR;
        Notify();
    });
    ArduinoOTA.begin();

    active = true;
    currentStatus.phase = EOTAUpdatePhase::AP_STARTED;
    Notify();
    LOG_DEBUG("SelfOTA: AP up at 192.168.4.1");
}

/**
 * @brief Take the access point down. Safe when it was never up.
 */
void SelfOTA::Stop() {
    if (!active) {
        return;
    }
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    active = false;
    LOG_DEBUG("SelfOTA: AP stopped");
}

/**
 * @brief Pump ArduinoOTA. No-op unless the access point is up.
 * @warning Blocks for the whole image once an upload starts.
 */
void SelfOTA::Handle() {
    if (!active) {
        return;
    }
    ArduinoOTA.handle();
    if (currentStatus.phase == EOTAUpdatePhase::AP_STARTED) {
        const uint32_t now = millis();
        if (now - lastHeartbeatMs >= AP_HEARTBEAT_MS) {
            lastHeartbeatMs = now;
            LOG_DEBUG("Heartbeat: AP started");
            Notify();
        }
    }
}

/**
 * @brief Report the current status to the callback.
 */
void SelfOTA::Notify() {
    if (onStatus) {
        onStatus(ECanNode::NODE_AIRRIDE_GUI, currentStatus);
    }
}
