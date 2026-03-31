#include <Arduino.h>
#include "LogHandler.h"
#include "PressureSensorManager.h"
#include "Settings.h"
#include "SolenoidManager.h"
#include "MainStateMachine.h"
#include "PressureSensor.h"
#include "Solenoid.h"
#include "MainCommunication.h"
#include "CanBus.h"

void SetupHardware();
void RegisterSensors();
void RegisterSolenoids();
void InitializeServices();

Settings settings;

Solenoid frontDownSolenoid(ESolenoid::FRONT_DOWN, PIN_D4);
Solenoid frontUpSolenoid(ESolenoid::FRONT_UP, PIN_D5);
Solenoid backUpSolenoid(ESolenoid::BACK_UP, PIN_D6);
Solenoid backDownSolenoid(ESolenoid::BACK_DOWN, PIN_D7);
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

CanQueue canQueue;
CanBus canBus(canQueue);
LargeCanMessageHandler largeCanMessageHandler(canBus);
Communication communication(canBus, canQueue, largeCanMessageHandler, ECanNode::NODE_AIRRIDE_CONTROLLER);

LogHandlerCommunication logHandlerCommunication(communication);
LogHandler logHandler(logHandlerCommunication, frontPressureSensor, backPressureSensor, tankPressureSensor);

MainStateMachineData mainStateMachineData;
MainStateMachineCommunication mainStateMachineCommunication(communication, mainStateMachineData);
MainStateMachine mainStateMachine(mainStateMachineData, mainStateMachineCommunication, solenoidManager,
                                  pressureSensorManager, logHandler, settings);

MainCommunication mainCommunication(communication, settings);

unsigned long timePrevious = 0;
constexpr int sendSensorInterval = 200;

void setup() {
  SetupHardware();
  RegisterSensors();
  RegisterSolenoids();
  InitializeServices();
}

void loop() {
  if (millis() - timePrevious > sendSensorInterval) {
    pressureSensorManager.Update();
    mainCommunication.SendPressure(frontPressureSensor.GetRawPressure(), backPressureSensor.GetRawPressure());
    timePrevious = millis();
  }
  communication.CheckForMessage();
  mainStateMachine.Loop();
  logHandler.SendLog();
}

void SetupHardware() {
  Serial.begin(9600);
  analogReadResolution(14);
  canBus.Setup(CAN1TX, CAN1RX, ECanBitRate::B500k);
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
  mainStateMachine.Begin();
  mainCommunication.Init();
}

