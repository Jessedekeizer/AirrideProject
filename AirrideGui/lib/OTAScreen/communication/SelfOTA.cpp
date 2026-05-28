#include "SelfOTA.h"
#include "Logger.h"
#include <WiFi.h>
#include <ArduinoOTA.h>

void SelfOTA::Init(StatusCallback cb) {
    onStatus = cb;
}

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
        currentStatus.progress = (uint8_t)(progress * 100 / total);
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

void SelfOTA::Stop() {
    if (!active) return;
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
    active = false;
    LOG_DEBUG("SelfOTA: AP stopped");
}

void SelfOTA::Handle() {
    if (active) ArduinoOTA.handle();
}

void SelfOTA::Notify() {
    if (onStatus) onStatus(ECanNode::NODE_AIRRIDE_GUI, currentStatus);
}
