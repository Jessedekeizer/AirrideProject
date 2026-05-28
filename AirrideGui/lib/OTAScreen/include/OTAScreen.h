#ifndef OTASCREEN_H
#define OTASCREEN_H

#include <vector>
#include "BaseScreen.h"
#include "ScreenManager.h"
#include "DisplayService.h"
#include "OTACommunication.h"
#include "CanMessages.h"
#include "CanMessageIds.h"

enum class EOTAState {
    SCANNING,
    SELECT,
    UPDATING,
    ERROR,
    DONE
};

class OTAScreen : public BaseScreen {
public:
    OTAScreen(ScreenManager &screenManager, DisplayService &displayService, OTACommunication &otaCommunication);
    void OnSetup() override;
    void OnLoop() override;

private:
    void OnBtnLeft(Button &btn);
    void OnBtnRight(Button &btn);
    void OnBtnStart(Button &btn);
    void OnBtnBack(Button &btn);

    void StartScan();
    void TransitionTo(EOTAState newState);
    void StartOTA();
    void StopOTA();
    void ExitScreen();

    void AddNodeToList(ECanNode node);
    void CheckForUpdates(const CANAirRideOTAStatus &status);

    void DrawScanningScreen();
    void DrawSelectScreen();
    void DrawUpdatingScreen();
    void DrawErrorScreen();
    void DrawDoneScreen();
    void DrawProgressBar(uint8_t percent);
    void DrawStartButton(const char *label);
    void ClearScreen();
    void ClearSelectScreen();


    static const char *NodeName(ECanNode node);

    ScreenManager &screenManager;
    DisplayService &displayService;
    OTACommunication &otaCommunication;

    EOTAState state;
    std::vector<ECanNode> discoveredNodes;
    int selectedIndex = 0;
    CANAirRideOTAStatus lastStatus{};

    unsigned long scanStartTime = 0;
    unsigned long doneTime = 0;
    unsigned long lastStatusTime = 0;
    uint8_t drawnProgressSection = 255;
    static constexpr unsigned long SCAN_TIMEOUT_MS    = 3000;
    static constexpr unsigned long DONE_RESCAN_MS     = 5000;
    static constexpr unsigned long UPDATE_TIMEOUT_MS  = 15000;
};

#endif
