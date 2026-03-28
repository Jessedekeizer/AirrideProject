#include "SettingsScreenCommunication.h"
#include "CANMessages.h"
#include "CanMessageIDs.h"
#include "Logger.h"

void SettingsScreenCommunication::SendSettings(SettingsDevice &settings) {
    CANSettingsAirRide canSettingsAirRide{
        settings.frontMax,
        settings.backMax,
        settings.rideFront,
        settings.rideBack,
        settings.frontUpX,
        settings.frontDownX, settings.backUpX,
        settings.backDownX,
        settings.parkDuration
    };
    communication.SendCANMessage(ECanNode::NODE_AIRRIDE_CONTROLLER, ECanMsgType::CAN_AIRRIDE_SETTINGS, canSettingsAirRide);
}
