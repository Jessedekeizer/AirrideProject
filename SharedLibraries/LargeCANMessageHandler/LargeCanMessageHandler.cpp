#include "LargeCanMessageHandler.h"
#include <algorithm>
#include <cmath>
#include <cstring>
#include "Logger.h"

#define MAX_CAN_DATA_LENGTH 8
#define DELAY_BETWEEN_SENDING_MS 5

void LargeCanMessageHandler::SendLargeMessage(CanId &canId, const uint8_t *data, uint8_t length) {
    int amountOfMessages = (length + MAX_CAN_DATA_LENGTH - 1) / MAX_CAN_DATA_LENGTH;

    for (int i = 0; i < amountOfMessages; i++) {
        int dlc = MAX_CAN_DATA_LENGTH;

        if (i == 0) {
            canId.flags = ECanFlags::FLAG_FIRST;
        }
        else if (i == amountOfMessages - 1) {
            canId.flags = ECanFlags::FLAG_LAST;

            int remaining = length % MAX_CAN_DATA_LENGTH;
            dlc = (remaining == 0) ? MAX_CAN_DATA_LENGTH : remaining;
        }
        else {
            canId.flags = ECanFlags::FLAG_LARGE_MESSAGE;
        }

        CanMessage canMessage{};
        canMessage.id = canId.ToRaw();
        canMessage.dlc = dlc;
        memcpy(canMessage.data, data + i * MAX_CAN_DATA_LENGTH, dlc);

        canBus.SendMessage(canMessage);
        delay(DELAY_BETWEEN_SENDING_MS);
    }
}

LargeCanMessage *LargeCanMessageHandler::HandleLargeCanMessage(const CanMessage &message) {
    CanId canId{};
    canId.FromRaw(message.id);
    LargeCanMessage *largeCanMessage = GetLargeCanMessage(canId.src, canId.type);
    switch (canId.flags) {
        case ECanFlags::FLAG_FIRST: {
            if (largeCanMessage) {
                RemoveLargeMessage(canId.src, canId.type);
            }
            CreateNewLargeMessage(canId, message);
            break;
        }
        case ECanFlags::FLAG_LARGE_MESSAGE: {
            if (largeCanMessage) {
                AppendMessageToLargeMessage(largeCanMessage, message);
            }
            break;
        }
        case ECanFlags::FLAG_LAST: {
            if (largeCanMessage) {
                AppendMessageToLargeMessage(largeCanMessage, message);
                return largeCanMessage;
            }
        }
        default: {
            break;
        }
    }
    return nullptr;
}

LargeCanMessage *LargeCanMessageHandler::GetLargeCanMessage(ECanNode sender, ECanMsgType msgType) {
    auto it = std::find_if(largeCANMessages.begin(), largeCANMessages.end(),
                           [sender, msgType](const LargeCanMessage &largeCanMessage) {
                               return largeCanMessage.id.type == msgType && largeCanMessage.id.src == sender;
                           });

    if (it != largeCANMessages.end()) {
        LOG_DEBUG("Found large message at:", static_cast<uint8_t>(it->id.src), static_cast<uint16_t>(it->id.type));
        return &(*it);
    }
    LOG_DEBUG("No large message found");
    return nullptr;
}

void LargeCanMessageHandler::RemoveLargeMessage(ECanNode sender, ECanMsgType msgType) {
    auto it = std::find_if(largeCANMessages.begin(), largeCANMessages.end(),
                           [sender, msgType](const LargeCanMessage &largeCanMessage) {
                               return largeCanMessage.id.type == msgType && largeCanMessage.id.src == sender;
                           });
    LOG_DEBUG("Removing large message src:", static_cast<uint8_t>(it->id.src), "type",
              static_cast<uint16_t>(it->id.type));
    largeCANMessages.erase(it);
}

void LargeCanMessageHandler::CreateNewLargeMessage(CanId &canId, const CanMessage &message) {
    LargeCanMessage largeCanMessage{};
    largeCanMessage.id = canId;
    AppendMessageToLargeMessage(&largeCanMessage, message);
    largeCANMessages.push_back(largeCanMessage);
}

void LargeCanMessageHandler::AppendMessageToLargeMessage(LargeCanMessage *largeCanMessage, const CanMessage &message) {
    for (int i = 0; i < message.dlc; ++i) {
        largeCanMessage->data.push_back(message.data[i]);
    }
    largeCanMessage->length = static_cast<uint8_t>(largeCanMessage->data.size());
}


