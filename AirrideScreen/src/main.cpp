#include <Arduino.h>
#include <XPT2046_Bitbang.h>

#include "CANOverSerial.h"
#include "Communication.h"
#include "ScreenManager.h"

#include "MainScreenData.h"
#include "MainScreenCommunication.h"
#include "MainScreen.h"

#include "SettingsScreenCommunication.h"
#include "Settings1Screen.h"
#include "Settings2Screen.h"
#include "Settings3Screen.h"
#include "Settings4Screen.h"
#include "CalibrationScreen.h"

#include "Logger.h"
#include "TimerManager.h"
#include "SdCardService.h"
#include "DisplayService.h"
#include "LogStorage.h"
#include "SettingsStorage.h"
#include "../lib/CanBus/CanBus.h"

#define MOSI_PIN 32
#define MISO_PIN 39
#define CLK_PIN 25
#define CS_PIN 33

#define SERIAL_BAUD_RATE 9600
#define SERIAL2_RX_PIN 27
#define SERIAL2_TX_PIN 22

XPT2046_Bitbang touchScreen(MOSI_PIN, MISO_PIN, CLK_PIN, CS_PIN);

void printTouchToSerial(TouchPoint);

void UpdateTouchScreen();

Timer *tickTimer = nullptr;

SPIClass spiSD = SPIClass(VSPI);

SdCardService sdCardService(SS, spiSD);
DisplayService displayService(sdCardService);
LogStorage logStorage(sdCardService);
SettingsStorage settingsStorage(sdCardService);

ScreenManager screenManager(displayService);

SettingsDevice settings;

CANQueue stringQueue;

CanBus canBus(stringQueue);
LargeCanMessageHandler largeCanMessageHandler(canBus);
Communication communication(canBus, stringQueue, largeCanMessageHandler, CanNode::NODE_ESP32);

MainScreenData mainScreenData;
MainScreenCommunication mainScreenCommunication(communication, mainScreenData, logStorage);
MainScreen mainScreen(mainScreenData, mainScreenCommunication, screenManager, settings, displayService, logStorage);

SettingsScreenCommunication settingsScreenCommunication(communication, settings);
Settings1Screen settings1Screen(screenManager, settingsScreenCommunication, settings, settingsStorage, displayService);
Settings2Screen settings2Screen(screenManager, settingsScreenCommunication, settings, settingsStorage, displayService);
Settings3Screen settings3Screen(screenManager, settingsScreenCommunication, settings, settingsStorage, displayService);
Settings4Screen settings4Screen(screenManager, settingsScreenCommunication, settings, settingsStorage, displayService);

CalibrationScreen calibrationScreen(screenManager, settings, settingsStorage, displayService, touchScreen);

void setup() {
  sdCardService.Begin();
  displayService.Begin();
  Serial.begin(SERIAL_BAUD_RATE, SERIAL_8N1);
  //Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SERIAL2_RX_PIN, SERIAL2_TX_PIN);
  canBus.Setup(SERIAL2_TX_PIN, SERIAL2_RX_PIN, ECanBitRate::B500k);

  settingsStorage.ReadSettings(settings);
  settingsScreenCommunication.SendSettings(settings);

  screenManager.AddScreen(&mainScreen);
  screenManager.AddScreen(&settings1Screen);
  screenManager.AddScreen(&settings2Screen);
  screenManager.AddScreen(&settings3Screen);
  screenManager.AddScreen(&settings4Screen);
  screenManager.AddScreen(&calibrationScreen);

  delay(1000);
  touchScreen.begin();
  timerManager.GetInstance();

  if (!settings.calibrationSet) {
    screenManager.RequestScreen(EScreen::CALIBRATION);
  } else {
    touchScreen.setCalibration(settings.xmin, settings.xmax, settings.ymin, settings.ymax);
    screenManager.RequestScreen(EScreen::MAIN);
  }
  tickTimer = new Timer(0.1, []() { UpdateTouchScreen(); }, true);
  timerManager.addTimer(tickTimer);
}

//====================================================================================
//                                    Loop
//====================================================================================
void loop() {
  timerManager.update();
  communication.CheckForMessage();
}

void UpdateTouchScreen() {
  TouchPoint touch = touchScreen.getTouch();
  if (touch.zRaw != 0) {
    // printTouchToSerial(touch);
    screenManager.GetActiveScreen()->HandleTouch(touch.x, touch.y);
  } else {
    screenManager.GetActiveScreen()->ReleaseButtons();
  }
  screenManager.GetActiveScreen()->OnLoop();
}

void printTouchToSerial(TouchPoint p) {
  LOG_DEBUG("Touch: X=", p.x, ", Y=", p.y, ", ZRaw=", p.zRaw);
}
