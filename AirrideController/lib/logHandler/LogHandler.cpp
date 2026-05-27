#include "LogHandler.h"
#include "Arduino.h"

constexpr int timeInterval = 100;

LogHandler::LogHandler(LogHandlerCommunication &communication, PressureSensor &frontPressureSensor,
                       PressureSensor &backPressureSensor, PressureSensor &tankPressureSensor)
    : communication(communication), frontPressureSensor(frontPressureSensor), backPressureSensor(backPressureSensor),
      tankPressureSensor(tankPressureSensor) {
}

LogHandler::~LogHandler() {
}

void LogHandler::StartFrontLog(bool together) {
    sendLogFront = false;
    startPressureFront = frontPressureSensor.GetRawPressure();
    startTankPressureFront = tankPressureSensor.GetRawPressure();
    startTimeFront = millis();
    togetherMoveFront = together;
}

void LogHandler::EndFrontLog() {
    sendLogFront = true;
    frontLogPreviousTime = millis();
}

void LogHandler::StartBackLog(bool together) {
    sendLogBack = false;
    startPressureBack = backPressureSensor.GetRawPressure();
    startTankPressureBack = tankPressureSensor.GetRawPressure();
    startTimeBack = millis();
    togetherMoveBack = together;
}

void LogHandler::EndBackLog() {
    sendLogBack = true;
    backLogPreviousTime = millis();
}

void LogHandler::SendAxisLog(bool front, PressureSensor &sensor, float startPressure, float startTankPressure,
                              unsigned long startTime, unsigned long previousTime, bool togetherMove, bool &sendFlag,
                              unsigned long now) {
    if (!sendFlag || now - previousTime <= timeInterval) return;
    float endPressure = sensor.GetRawPressure();
    bool direction = startPressure - endPressure < 0;
    communication.SendLog(front, startPressure, endPressure, startTankPressure,
                          (now - startTime - timeInterval), direction, togetherMove);
    sendFlag = false;
}

void LogHandler::SendLog() {
    unsigned long now = millis();
    SendAxisLog(true,  frontPressureSensor, startPressureFront, startTankPressureFront,
                startTimeFront, frontLogPreviousTime, togetherMoveFront, sendLogFront, now);
    SendAxisLog(false, backPressureSensor,  startPressureBack,  startTankPressureBack,
                startTimeBack,  backLogPreviousTime,  togetherMoveBack,  sendLogBack,  now);
}
