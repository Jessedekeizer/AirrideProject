#include "OTAState.h"
#include "Logger.h"

#define PROGRESS_MAX_VALUE 100
#define PROGRESS_MIN_VALUE 0
#define PROGRESS_INCREMENT_VALUE 10

static OTAState *otaStateInstance = nullptr;

OTAState::OTAState(SolenoidManager &solenoidManager, OTACommunication &otaCommunication)
    : solenoidManager(solenoidManager), otaCommunication(otaCommunication) {}

EMainState OTAState::GetState() {
    return EMainState::OTA;
}

#ifdef ARDUINO_UNOWIFIR4
#include <WiFiS3.h>
#include <ArduinoOTA.h>

void OTAState::ProgressTimerCallback(TimerHandle_t xTimer) {
    if (otaStateInstance && otaStateInstance->internalState == EOTAState::FLASHING) {
        if (otaStateInstance->progress < PROGRESS_MAX_VALUE) {
            otaStateInstance->progress += PROGRESS_INCREMENT_VALUE;
        }
        otaStateInstance->otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::FLASHING, otaStateInstance->progress);
    }
}

void OTAState::OnOTAStart() {
    LOG_INFO("OTA update starting");
    if (!otaStateInstance) return;
    otaStateInstance->internalState = EOTAState::FLASHING;
    otaStateInstance->otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::FLASHING, PROGRESS_MAX_VALUE);
    if (otaStateInstance->progressTimer)
        xTimerStart(otaStateInstance->progressTimer, 0);
}

void OTAState::OnOTABeforeApply() {
    LOG_INFO("OTA update complete, applying");
    if (!otaStateInstance) return;
    if (otaStateInstance->progressTimer)
        xTimerStop(otaStateInstance->progressTimer, 0);
    otaStateInstance->internalState = EOTAState::DONE;
    otaStateInstance->otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::COMPLETE, PROGRESS_MAX_VALUE);
    otaStateInstance->endAccessPoint();
}

void OTAState::OnOTAError(int code, const char *msg) {
    LOG_ERROR("OTA error ", code, ": ", msg);
    if (!otaStateInstance) return;
    if (otaStateInstance->progressTimer)
        xTimerStop(otaStateInstance->progressTimer, 0);
    otaStateInstance->internalState = EOTAState::DONE;
    otaStateInstance->otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::ERROR, PROGRESS_MIN_VALUE);
}

void OTAState::Enter() {
    LOG_INFO("Entering OTA state");
    LOG_DEBUG(WiFi.firmwareVersion());
    solenoidManager.TurnOffAllSolenoids();
    internalState = EOTAState::AP_LISTENING;
    otaStateInstance = this;

    progressTimer = xTimerCreate("OTAProgress", pdMS_TO_TICKS(PROGRESS_TIMER_MS), pdTRUE, nullptr, ProgressTimerCallback);

    int status = WiFi.beginAP(OTA_AP_SSID, OTA_AP_PASS);
    if (status != WL_AP_LISTENING) {
        LOG_ERROR("Failed to start WiFi AP");
        otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::ERROR, PROGRESS_MIN_VALUE);
        internalState = EOTAState::DONE;
        return;
    }

    ArduinoOTA.onStart(OnOTAStart);
    ArduinoOTA.beforeApply(OnOTABeforeApply);
    ArduinoOTA.onError(OnOTAError);
    ArduinoOTA.begin(WiFi.localIP(), "AirrideController", OTA_PASSWORD, InternalStorage);

    lastHeartbeatMs = millis();
    otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::AP_STARTED, PROGRESS_MIN_VALUE);
    LOG_INFO("OTA AP started: ", OTA_AP_SSID, " - 192.168.4.1");
}

EMainState OTAState::Loop() {
    if (internalState == EOTAState::DONE) return EMainState::SUSPENSION;
    ArduinoOTA.poll();

    if (internalState == EOTAState::AP_LISTENING) {
        unsigned long now = millis();
        if (now - lastHeartbeatMs >= HEARTBEAT_INTERVAL_MS) {
            lastHeartbeatMs = now;
            otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::AP_STARTED, PROGRESS_MIN_VALUE);
        }
    }

    return EMainState::OTA;
}

void OTAState::Leave() {
    LOG_INFO("Leaving OTA state");
    if (progressTimer) {
        xTimerDelete(progressTimer, 0);
        progressTimer = nullptr;
    }
    ArduinoOTA.end();
    endAccessPoint();
    otaStateInstance = nullptr;
    internalState = EOTAState::AP_LISTENING;
}

void OTAState::endAccessPoint() {
    using namespace std;
    string res = "";
    modem.begin();
    modem.write(string(PROMPT(_DISCONNECTSOFTAP)),res,"%s%d\r\n" ,CMD_WRITE(_DISCONNECTSOFTAP),1);
}

#else

void OTAState::Enter() {
    LOG_INFO("Entering OTA state");
    solenoidManager.TurnOffAllSolenoids();
    otaCommunication.SendStatus(EOTAStatusType::UPDATE, EOTAUpdatePhase::ERROR, 0);
    internalState = EOTAState::DONE;
}

EMainState OTAState::Loop() {
    if (internalState == EOTAState::DONE) return EMainState::SUSPENSION;
    return EMainState::OTA;
}

void OTAState::Leave() {
    LOG_INFO("Leaving OTA state");
    internalState = EOTAState::AP_LISTENING;
}

#endif
