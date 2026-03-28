#include "LargeCanMessageHandler.h"

#include <algorithm>
#include <cmath>
#include <cstring>

#include "../Logger/Logger.h"

LargeCanMessage *LargeCanMessageHandler::HandleLargeCanMessage(const CanMessage &message) {
    CanId canId{};
    canId.FromRaw(message.id);
    switch (canId.flags) {
        case ECanFlags::FLAG_FIRST: {
            LargeCanMessage largeCanMessage{};
            largeCanMessage.id = canId;
            AppendMessageToLargeMessage(largeCanMessage, message);
            largeCANMessages.push_back(largeCanMessage);
            break;
        }
        case ECanFlags::FLAG_LARGE_MESSAGE: {
            LargeCanMessage *largeCanMessage = GetLargeCanMessage(canId.src, canId.type);
            if (largeCanMessage == nullptr) {
                break;
            }
            AppendMessageToLargeMessage(*largeCanMessage, message);
            break;
        }
        case ECanFlags::FLAG_LAST: {
            LargeCanMessage *largeCanMessage = GetLargeCanMessage(canId.src, canId.type);
            if (largeCanMessage == nullptr) {
                break;
            }
            AppendMessageToLargeMessage(*largeCanMessage, message);
            return largeCanMessage;
        }
        default: {
            break;
        }
    }
    return nullptr;
}

void LargeCanMessageHandler::SendLargeMessage(CanId &canId, const uint8_t *data, uint8_t length) {
    int amountOfMessages = (length + 7) / 8;

    for (int i = 0; i < amountOfMessages; i++) {
        int dlc = 8;

        if (i == 0) {
            canId.flags = ECanFlags::FLAG_FIRST;
        } else if (i == amountOfMessages - 1) {
            canId.flags = ECanFlags::FLAG_LAST;

            int remaining = length % 8;
            dlc = (remaining == 0) ? 8 : remaining;
        } else {
            canId.flags = ECanFlags::FLAG_LARGE_MESSAGE;
        }

        CanMessage canMessage{};
        canMessage.id = canId.ToRaw();
        canMessage.dlc = dlc;
        memcpy(canMessage.data, data + i * 8, dlc);

        canBus.SendMessage(canMessage);
        delay(5);
    }
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

void LargeCanMessageHandler::AppendMessageToLargeMessage(LargeCanMessage &largeCanMessage, const CanMessage &message) {
    for (int i = 0; i < message.dlc; ++i) {
        largeCanMessage.data.push_back(message.data[i]);
    }
    largeCanMessage.length = static_cast<uint8_t>(largeCanMessage.data.size());
}


