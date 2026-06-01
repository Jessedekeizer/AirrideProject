#ifndef OTASTATE_H
#define OTASTATE_H
#include "EOTAState.h"
#include "IMainState.h"
#include "SolenoidManager.h"
#include "OTACommunication.h"

class OTAState : public IMainState {
public:
    OTAState(SolenoidManager &solenoidManager, OTACommunication &otaCommunication);

    EMainState GetState() override;
    void Enter() override;
    EMainState Loop() override;
    void Leave() override;

private:
#ifdef ARDUINO_UNOWIFIR4
    static void OnOTAStart();
    static void OnOTABeforeApply();
    static void OnOTAError(int code, const char *msg);
    static void ProgressTimerCallback(TimerHandle_t xTimer);
    TimerHandle_t progressTimer = nullptr;
    int progress = 0;
    void endAccessPoint();
#endif



    SolenoidManager &solenoidManager;
    OTACommunication &otaCommunication;
    EOTAState internalState = EOTAState::AP_LISTENING;
    unsigned long lastHeartbeatMs = 0;

    static constexpr unsigned long HEARTBEAT_INTERVAL_MS = 5000;
    static constexpr unsigned long PROGRESS_TIMER_MS = 1000;
};

#endif //OTASTATE_H
