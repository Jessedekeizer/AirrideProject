#ifndef LOGHANDLER_H
#define LOGHANDLER_H
#include "PressureSensor.h"
#include "MainCommunication.h"
#include "LogHandlerCommunication.h"

class LogHandler {
public:
    LogHandler(LogHandlerCommunication &communication, PressureSensor &frontPressureSensor,
               PressureSensor &backPressureSensor, PressureSensor &tankPressureSensor);

    ~LogHandler();

    void StartFrontLog(bool togetherMove = false);

    void EndFrontLog();

    void StartBackLog(bool togetherMove = false);

    void EndBackLog();

    void SendLog();

private:
    unsigned long startTimeFront = 0;
    float startPressureFront = 0;
    float startTankPressureFront = 0;
    bool togetherMoveFront = false;
    bool sendLogFront = false;

    unsigned long startTimeBack = 0;
    float startPressureBack = 0;
    float startTankPressureBack = 0;
    bool togetherMoveBack = false;
    bool sendLogBack = false;

    unsigned long frontLogPreviousTime = 0;
    unsigned long backLogPreviousTime = 0;

    LogHandlerCommunication &communication;

    PressureSensor &frontPressureSensor;
    PressureSensor &backPressureSensor;
    PressureSensor &tankPressureSensor;
};
#endif
