#include "SettingsScreenCommunication.h"
#include "CANMessages.h"
#include "CANMessageIDs.h"
#include "Logger.h"

void SettingsScreenCommunication::SendSettings(SettingsDevice &settings) {
    CANSettingsAirRide canSettingsAirRide{settings.frontMax, settings.backMax, settings.rideFront, settings.rideBack, settings.frontUpX, settings.frontDownX, settings.backUpX, settings.frontDownX, settings.parkDuration};
    communication.SendCANMessage(CanNode::NODE_R4, CanMsgType::CAN_AIRRIDE_SETTINGS, canSettingsAirRide);
}
