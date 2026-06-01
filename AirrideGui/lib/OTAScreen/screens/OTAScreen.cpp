#include "OTAScreen.h"
#include "EOTAScreenButtons.h"
#include "OTAScreenGeometry.h"
#include "Logger.h"

OTAScreen::OTAScreen(ScreenManager &screenManager, DisplayService &displayService, OTACommunication &otaCommunication)
    : BaseScreen(EScreen::OTA, SCREEN_PATH),
      screenManager(screenManager),
      displayService(displayService),
      otaCommunication(otaCommunication),
      state(EOTAState::SCANNING),
      discoveredNodes() {
    buttons.push_back(new PushButton(OTA_BTN_LEFT_X,  OTA_BTN_LEFT_Y,  OTA_BTN_LEFT_W,  OTA_BTN_LEFT_H,  SCROLL_LEFT,  [this](Button &b) { OnBtnLeft(b);  }));
    buttons.push_back(new PushButton(OTA_BTN_RIGHT_X, OTA_BTN_RIGHT_Y, OTA_BTN_RIGHT_W, OTA_BTN_RIGHT_H, SCROLL_RIGHT, [this](Button &b) { OnBtnRight(b); }));
    buttons.push_back(new PushButton(OTA_BTN_START_X, OTA_BTN_START_Y, OTA_BTN_START_W, OTA_BTN_START_H, START, [this](Button &b) { OnBtnStart(b); }));
    buttons.push_back(new PushButton(OTA_BTN_BACK_X,  OTA_BTN_BACK_Y,  OTA_BTN_BACK_W,  OTA_BTN_BACK_H,  EXIT,  [this](Button &b) { OnBtnBack(b);  }));
}

void OTAScreen::OnSetup() {
    auto statusCallback = [this](ECanNode node, const CANAirRideOTAStatus &status) {
        switch (status.type) {
            case EOTAStatusType::AVAILABLE:
                if (state != EOTAState::SCANNING)
                    return;
                AddNodeToList(node);
                break;
            case EOTAStatusType::UPDATE: {
                if (state != EOTAState::UPDATING)
                    return;
                CheckForUpdates(status);
                break;
            }
            default:
                LOG_ERROR("OTAScreen: unknown status type: ", (uint8_t)status.type);
                break;
        }
    };
    otaCommunication.Init(statusCallback);
    StartScan();
}

void OTAScreen::AddNodeToList(ECanNode node) {
    for (ECanNode n : discoveredNodes) {
        if (n == node) return;
    }
    discoveredNodes.push_back(node);
    LOG_DEBUG("OTAScreen: discovered node ", (int)node);
}

void OTAScreen::CheckForUpdates(const CANAirRideOTAStatus &status) {
    bool phaseChanged = status.phase != lastStatus.phase;
    uint8_t section = status.progress / 10;
    bool progressChanged = section != drawnProgressSection;
    lastStatus = status;
    lastStatusTime = millis();
    switch (status.phase) {
        case EOTAUpdatePhase::ERROR:
            TransitionTo(EOTAState::ERROR);
            break;
        case EOTAUpdatePhase::COMPLETE:
            TransitionTo(EOTAState::DONE);
            break;
        default:
            if (phaseChanged || progressChanged) {
                drawnProgressSection = section;
                DrawUpdatingScreen();
            }
            break;
    }
}

void OTAScreen::OnLoop() {
    switch (state) {
        case EOTAState::SCANNING:
            if (millis() - scanStartTime >= SCAN_TIMEOUT_MS) {
                TransitionTo(EOTAState::SELECT);
            }
            break;
        case EOTAState::UPDATING:
            otaCommunication.Handle();
            if (lastStatusTime > 0 && millis() - lastStatusTime >= UPDATE_TIMEOUT_MS) {
                TransitionTo(EOTAState::ERROR);
            }
            break;
        case EOTAState::DONE:
            if (millis() - doneTime >= DONE_RESCAN_MS) {
                StartScan();
            }
            break;
        default:
            break;
    }
}

void OTAScreen::StartScan() {
    discoveredNodes.clear();
    discoveredNodes.push_back(ECanNode::NODE_AIRRIDE_GUI);
    selectedIndex = 0;
    lastStatus = {};
    scanStartTime = millis();
    otaCommunication.SendDiscover();
    TransitionTo(EOTAState::SCANNING);
}

void OTAScreen::TransitionTo(EOTAState newState) {
    state = newState;
    switch (state) {
        case EOTAState::SCANNING:
            DrawScanningScreen();
            break;
        case EOTAState::SELECT:
            DrawSelectScreen();
            break;
        case EOTAState::UPDATING:
            DrawUpdatingScreen();
            break;
        case EOTAState::ERROR:
            DrawErrorScreen();
            break;
        case EOTAState::DONE:
            doneTime = millis();
            DrawDoneScreen();
            break;
        default:
            LOG_ERROR("OTAScreen: unknown state: ", (int)state);
            break;
    }
}

void OTAScreen::StartOTA() {
    lastStatus = {};
    drawnProgressSection = 255;
    lastStatusTime = 0;
    TransitionTo(EOTAState::UPDATING);
    otaCommunication.SendStart(discoveredNodes[selectedIndex]);
}

void OTAScreen::StopOTA() {
    if (!discoveredNodes.empty()) {
        otaCommunication.SendStop(ECanNode::NODE_BROADCAST);
    }
}

void OTAScreen::ExitScreen() {
    otaCommunication.Leave();
    screenManager.RequestScreen(EScreen::MAIN);
}

void OTAScreen::OnBtnLeft(Button &) {
    if (state != EOTAState::SELECT || discoveredNodes.empty()) return;
    selectedIndex = (selectedIndex == 0) ? (int)discoveredNodes.size() - 1 : selectedIndex - 1;
    DrawSelectScreen();
}

void OTAScreen::OnBtnRight(Button &) {
    if (state != EOTAState::SELECT || discoveredNodes.empty()) return;
    selectedIndex = (selectedIndex == (int)discoveredNodes.size() - 1) ? 0 : selectedIndex + 1;
    DrawSelectScreen();
}

void OTAScreen::OnBtnStart(Button &) {
    if (state == EOTAState::SELECT && !discoveredNodes.empty()) StartOTA();
    else if (state == EOTAState::ERROR) StartScan();
}

void OTAScreen::OnBtnBack(Button &) {
    StopOTA();

    ExitScreen();
}

void OTAScreen::DrawStartButton(const char *label) {
    displayService.DrawRectangle(OTA_BTN_START_X, OTA_BTN_START_Y, OTA_BTN_START_W, OTA_BTN_START_H, TFT_DARKGREEN);
    displayService.DrawText(label, OTA_BTN_START_LABEL_X, OTA_BTN_START_LABEL_Y, 2, TFT_WHITE, TFT_DARKGREEN);
}

void OTAScreen::ClearScreen() {
    displayService.DrawRectangle(0, 63, OTA_SCR_W, OTA_SCR_H, TFT_BLACK);
}

void OTAScreen::ClearSelectScreen() {
    displayService.DrawImage(SCREEN_PATH);
}


void OTAScreen::DrawScanningScreen() {
    ClearScreen();
    displayService.DrawText("Scanning for", OTA_SCAN_LINE1_X, OTA_SCAN_LINE1_Y, 2);
    displayService.DrawText("devices...", OTA_SCAN_LINE2_X, OTA_SCAN_LINE2_Y, 2);
}

void OTAScreen::DrawSelectScreen() {
    ClearSelectScreen();

    if (discoveredNodes.empty()) {
        displayService.DrawText("No devices found", OTA_SELECT_NONE_X, OTA_SELECT_NONE_Y, 2);
        DrawStartButton("RETRY");
    } else {
        displayService.DrawText("Select Device:", OTA_SELECT_TITLE_X, OTA_SELECT_TITLE_Y, 2);

        displayService.DrawText(NodeName(discoveredNodes[selectedIndex]), OTA_NODE_NAME_X, OTA_NODE_NAME_Y, 4);

        char counter[16];
        snprintf(counter, sizeof(counter), "%d / %d", selectedIndex + 1, (int)discoveredNodes.size());
        displayService.DrawText(counter, OTA_SELECT_COUNTER_X, OTA_SELECT_COUNTER_Y, 2);

        DrawStartButton("START OTA");
    }
}

void OTAScreen::DrawUpdatingScreen() {
    ClearScreen();

    char line[32];
    if (!discoveredNodes.empty()) {
        snprintf(line, sizeof(line), "Updating: %s", NodeName(discoveredNodes[selectedIndex]));
        displayService.DrawText(line, OTA_UPDATE_NODE_X, OTA_UPDATE_NODE_Y, 2);
    }

    const char *phaseStr = "IDLE";
    switch (lastStatus.phase) {
        case EOTAUpdatePhase::AP_STARTED: phaseStr = "AP UP";    break;
        case EOTAUpdatePhase::FLASHING:   phaseStr = "FLASHING"; break;
        case EOTAUpdatePhase::COMPLETE:   phaseStr = "DONE";     break;
        case EOTAUpdatePhase::ERROR:      phaseStr = "ERROR";    break;
        default: break;
    }
    displayService.DrawText(phaseStr, OTA_UPDATE_STATUS_X, OTA_UPDATE_STATUS_Y, 1);

    DrawProgressBar(lastStatus.progress);

    char pct[8];
    snprintf(pct, sizeof(pct), "%d%%", lastStatus.progress);
    displayService.DrawText(pct, OTA_PROGRESS_PCT_X, OTA_PROGRESS_PCT_Y, 2);
}

void OTAScreen::DrawProgressBar(uint8_t percent) {
    if (percent > 100) percent = 100;
    displayService.DrawRectangle(OTA_PROGRESS_X, OTA_PROGRESS_Y, OTA_PROGRESS_W, OTA_PROGRESS_H, TFT_DARKGREY);
    int fillW = (OTA_PROGRESS_W - 2) * percent / 100;
    if (fillW > 0) {
        uint32_t color = (percent < 100) ? TFT_GREEN : TFT_CYAN;
        displayService.DrawRectangle(OTA_PROGRESS_X + 1, OTA_PROGRESS_Y + 1, fillW, OTA_PROGRESS_H - 2, color);
    }
}

void OTAScreen::DrawErrorScreen() {
    ClearScreen();
    displayService.DrawText("OTA Failed!", OTA_ERROR_X, OTA_ERROR_Y, 3, TFT_RED, TFT_BLACK);
    DrawStartButton("RETRY");
}

void OTAScreen::DrawDoneScreen() {
    ClearScreen();
    displayService.DrawText("Update Complete!", OTA_DONE_X, OTA_DONE_Y, 2, TFT_GREEN, TFT_BLACK);
}

const char *OTAScreen::NodeName(ECanNode node) {
    switch (node) {
        case ECanNode::NODE_AIRRIDE_CONTROLLER: return "Controller";
        case ECanNode::NODE_AIRRIDE_GUI:        return "GUI";
        default:                                return "Unknown";
    }
}
