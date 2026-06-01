#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "SuspensionStateMachine.h"
#include "PressureSensor.h"
#include "MainCommunication.h"
#include "OTACommunication.h"
#include "CanBus.h"
#include "SuspensionStateCommunication.h"
#include "MainStateMachine.h"
#include <Arduino_FreeRTOS.h>
#ifdef TEST_SOLENOIDS
#include "LedMatrixSolenoid.h"
#else
#include "Solenoid.h"
#endif

void SetupHardware();
void RegisterSensors();
void RegisterSolenoids();
void InitializeServices();
void MainTask(void *arg);

Settings settings;

#ifdef TEST_SOLENOIDS
LedMatrixSolenoid frontDownSolenoid(ESolenoid::FRONT_DOWN, 3);
LedMatrixSolenoid frontUpSolenoid(ESolenoid::FRONT_UP, 0);
LedMatrixSolenoid backUpSolenoid(ESolenoid::BACK_UP, 6);
LedMatrixSolenoid backDownSolenoid(ESolenoid::BACK_DOWN, 9);
#else
Solenoid frontDownSolenoid(ESolenoid::FRONT_DOWN, PIN_D4);
Solenoid frontUpSolenoid(ESolenoid::FRONT_UP, PIN_D5);
Solenoid backUpSolenoid(ESolenoid::BACK_UP, PIN_D6);
Solenoid backDownSolenoid(ESolenoid::BACK_DOWN, PIN_D7);
#endif
SolenoidManager solenoidManager;

constexpr float analogMin = 1638.4f;
constexpr float analogMax = 14745.6f;
constexpr float barMax = 14.82f;
constexpr float barTankMax = 15.2f;
constexpr int frontFilterSize = 16;
constexpr int backFilterSize = 10;

PressureSensor frontPressureSensor(EPressureSensor::FRONT, A0, frontFilterSize, analogMin, analogMax, barMax);
PressureSensor backPressureSensor(EPressureSensor::BACK, A1, backFilterSize, analogMin, analogMax, barMax);
PressureSensor tankPressureSensor(EPressureSensor::TANK, A2, backFilterSize, analogMin, analogMax, barTankMax);
PressureSensorManager pressureSensorManager(frontUpSolenoid, backUpSolenoid, settings);

CanBus canBus;
LargeCanMessageHandler largeCanMessageHandler;
Communication communication(largeCanMessageHandler, ECanNode::NODE_AIRRIDE_CONTROLLER);

LogHandlerCommunication logHandlerCommunication(communication);
LogHandler logHandler(logHandlerCommunication, frontPressureSensor, backPressureSensor, tankPressureSensor);

SuspensionStateMachineData suspensionStateMachineData;
SuspensionStateMachineCommunication suspensionStateMachineCommunication(communication, suspensionStateMachineData);
SuspensionStateMachine suspensionStateMachine(suspensionStateMachineData, suspensionStateMachineCommunication,
                                              solenoidManager, pressureSensorManager, logHandler, settings);

OTACommunication otaCommunication(communication);
MainStateMachineData mainStateMachineData;
MainCommunication mainCommunication(communication, settings, mainStateMachineData, otaCommunication);

SuspensionStateCommunication suspensionStateCommunication(communication);
MainStateMachine mainStateMachine(mainStateMachineData,
                                                      suspensionStateMachine,
                                                      logHandler,
                                                      pressureSensorManager,
                                                      suspensionStateCommunication,
                                                      solenoidManager,
                                                      otaCommunication);

void setup() {
    SetupHardware();
    RegisterSensors();
    RegisterSolenoids();
    InitializeServices();

    xTaskCreate(MainTask, "Main", 512, nullptr, 1, nullptr);

    vTaskStartScheduler();
}

void loop() {}

void MainTask(void *arg) {
    (void) arg;

    for (;;) {
        communication.CheckForMessage();
        mainStateMachine.Loop();
        vTaskDelay(pdMS_TO_TICKS(1));
    }
}

void SetupHardware() {
    Serial.begin(9600);
    analogReadResolution(14);
    canBus.Setup(CANTX, CANRX, ECanBitRate::B500k);
    communication.SetQueues(canBus.GetRxQueue(), canBus.GetTxQueue());
}

void RegisterSensors() {
    pressureSensorManager.AddPressureSensor(frontPressureSensor);
    pressureSensorManager.AddPressureSensor(backPressureSensor);
    pressureSensorManager.AddPressureSensor(tankPressureSensor);
}

void RegisterSolenoids() {
    solenoidManager.AddSolenoid(frontDownSolenoid);
    solenoidManager.AddSolenoid(frontUpSolenoid);
    solenoidManager.AddSolenoid(backDownSolenoid);
    solenoidManager.AddSolenoid(backUpSolenoid);
}

void InitializeServices() {
    solenoidManager.Begin();
    pressureSensorManager.Begin();
    mainCommunication.Init();
    mainStateMachine.Begin();
}
