#include "SettingsScreenCommunication.h"
#include "CANMessages.h"
#include "CANMessageIDs.h"
#include "Logger.h"

void SettingsScreenCommunication::SendSettings(SettingsDevice &settings) {
    CANSettingsMax canSettingsMax{settings.frontMax, settings.backMax};
    CANSettingsRide canSettingsRide{settings.rideFront, settings.rideBack};
    CANSettingsFrontFactor canSettingsFrontFactor{settings.frontUpX, settings.frontDownX};
    CANSettingsBackFactor canSettingsBackFactor{settings.backUpX, settings.backDownX};
    CANSettingsPark canSettingsPark{settings.parkDuration};


    communication.SendCANMessage(static_cast<int16_t>(CAN_ID::CANSettingsMax), canSettingsMax);
    communication.SendCANMessage(static_cast<int16_t>(CAN_ID::CANSettingsRide), canSettingsRide);
    communication.SendCANMessage(static_cast<int16_t>(CAN_ID::CANSettingsFrontFactor), canSettingsFrontFactor);
    communication.SendCANMessage(static_cast<int16_t>(CAN_ID::CANSettingsBackFactor), canSettingsBackFactor);
    communication.SendCANMessage(static_cast<int16_t>(CAN_ID::CANSettingsPark), canSettingsPark);
}
