#include "LogHandler.h"
#include "Arduino.h"
#include "PressureSensorManager.h"

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

void LogHandler::SendLog() {
    if (sendLogFront && millis() - frontLogPreviousTime > timeInterval) {
        float endPressure = frontPressureSensor.GetRawPressure();
        bool directionFront = startPressureFront - endPressure < 0;
        communication.SendLog(true, startPressureFront, endPressure, startTankPressureFront,
                              (millis() - startTimeFront - timeInterval), directionFront,
                              togetherMoveFront);
        sendLogFront = false;
    }
    if (sendLogBack && millis() - backLogPreviousTime > timeInterval) {
        float endPressure = backPressureSensor.GetRawPressure();
        bool directionBack = startPressureFront - endPressure < 0;
        communication.SendLog(false, startPressureBack, endPressure, startTankPressureBack,
                              (millis() - startTimeBack - timeInterval), directionBack, togetherMoveBack);
        sendLogBack = false;
    }
}
