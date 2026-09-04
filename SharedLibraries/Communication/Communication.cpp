#include "Communication.h"
#include "Logger.h"
#include "CanID.h"
#include "LargeCanMessage.h"
#include "CanMessages.h"

static constexpr uint8_t MAX_CAN_DATA_LENGTH = 8;

Communication::Communication(LargeCanMessageHandler &largeCanMessageHandler, ECanNode me)
    : nextId(0), rxQueue(nullptr), txQueue(nullptr),
      largeCanMessageHandler(largeCanMessageHandler), me(me) {
}

Communication::~Communication() {
    subscribers.clear();
}

void Communication::SetQueues(QueueHandle_t rx, QueueHandle_t tx) {
    rxQueue = rx;
    txQueue = tx;
    largeCanMessageHandler.SetTxQueue(tx);
}

int Communication::Subscribe(Callback callback) {
    Subscription subscriber;
    subscriber.id = nextId++;
    subscriber.callback = std::move(callback);
    subscribers.push_back(subscriber);
    return subscriber.id;
}

void Communication::Unsubscribe(int id) {
    if (id == -1) {
        return;
    }
    for (int i = 0; i < subscribers.size(); i++) {
        if (subscribers[i].id == id) {
            subscribers.erase(subscribers.begin() + i);
            return;
        }
    }
}

void Communication::Notify(const CanId &canId, const uint8_t *data, uint8_t length) {
    for (const auto &subscriber: subscribers) {
        subscriber.callback(canId, data, length);
    }
}

void Communication::CheckForMessage() {
    if (rxQueue == nullptr) return;
    CanMessage message{};
    while (xQueueReceive(rxQueue, &message, 0) == pdTRUE) {
        DecodeCanMessage(message);
    }
    CheckPendingAcks();
}

void Communication::DecodeCanMessage(const CanMessage &message) {
    CanId canID;
    if (!canID.FromRaw(message.id)) {
        return;
    }
    if (!canID.IsForNode(me)) {
        return;
    }
    if (canID.HasFlag()) {
        LargeCanMessage *largeMessage = largeCanMessageHandler.HandleLargeCanMessage(message);
        if (largeMessage) {
            const uint8_t *payload = largeMessage->data.data();
            uint8_t payloadLength = largeMessage->length;
            // Ack before Notify: subscriber work (storage writes, etc) can be
            // slow and must not delay the ack, or the sender's retry timeout
            // fires before we even get a chance to reply.
            if (canID.AckRequired()) {
                ECanAckStatus status = VerifyAndStripCrc(payload, payloadLength);
                SendAck(canID.src, canID.type, status);
            }
            Notify(canID, payload, payloadLength);
            largeCanMessageHandler.RemoveLargeMessage(canID.src, canID.type);
        }
        return;
    }
    const uint8_t *payload = message.data;
    uint8_t payloadLength = message.dlc;
    if (canID.AckRequired()) {
        ECanAckStatus status = VerifyAndStripCrc(payload, payloadLength);
        SendAck(canID.src, canID.type, status);
    }
    Notify(canID, payload, payloadLength);
    if (canID.type == ECanMsgType::CAN_AIRRIDE_ACK) {
        HandleAckReceived(canID, message.data, message.dlc);
    }
}

void Communication::SendCanMessage(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length, bool requireAck) {
    if (txQueue == nullptr) return;

    // AckRequired sends carry a trailing CRC16 of the real payload, so the
    // receiver can tell corrupted/incomplete bytes from a clean receive
    // without knowing the struct behind this ECanMsgType.
    std::vector<uint8_t> withCrc;
    const uint8_t *sendData = data;
    uint8_t sendLength = length;
    if (requireAck) {
        uint16_t crc = ComputeCrc(data, length);
        withCrc.assign(data, data + length);
        withCrc.push_back(static_cast<uint8_t>(crc & 0xFF));
        withCrc.push_back(static_cast<uint8_t>((crc >> 8) & 0xFF));
        sendData = withCrc.data();
        sendLength = static_cast<uint8_t>(withCrc.size());
    }

    CanId canId;
    canId.src = me;
    canId.dst = target;
    canId.type = type;
    canId.flags = ECanFlags::FLAG_NONE;

    if (sendLength > MAX_CAN_DATA_LENGTH) {
        largeCanMessageHandler.SendLargeMessage(canId, sendData, sendLength, requireAck);
        if (requireAck) {
            TrackPendingAck(target, type, sendData, sendLength);
        }
        return;
    }

    if (requireAck) {
        canId.flags = ECanFlags::FLAG_ACK_REQUIRED;
    }

    CanMessage msg{};
    msg.id = canId.ToRaw();
    msg.dlc = sendLength;
    memcpy(msg.data, sendData, sendLength);

    if (xQueueSend(txQueue, &msg, pdMS_TO_TICKS(10)) != pdTRUE) {
        LOG_ERROR("TX: Queue full");
        return;
    }

    if (requireAck) {
        TrackPendingAck(target, type, sendData, sendLength);
    }
}

void Communication::SendAck(ECanNode target, ECanMsgType ackedType, ECanAckStatus status) {
    CanAckPayload payload{ackedType, status};
    // requireAck defaults false here - acks never trigger acks.
    SendCanMessage(target, ECanMsgType::CAN_AIRRIDE_ACK, payload);
}

void Communication::TrackPendingAck(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length) {
    PendingAck pending;
    pending.target = target;
    pending.type = type;
    pending.data.assign(data, data + length);
    pending.sentAt = millis();
    pending.retriesLeft = MAX_RETRIES;
    pendingAcks.push_back(std::move(pending));
}

void Communication::HandleAckReceived(const CanId &canId, const uint8_t *data, uint8_t length) {
    CanAckPayload ack{};
    if (!decodeCANMessage(data, length, ack)) return;

    for (size_t i = 0; i < pendingAcks.size(); i++) {
        if (pendingAcks[i].target == canId.src && pendingAcks[i].type == ack.type) {
            if (ack.status != ECanAckStatus::STATUS_OK) {
                // Receiver's CRC check failed - leave the entry in place so
                // it retries on the next CheckPendingAcks tick, same as a
                // lost/never-arrived ack.
                LOG_ERROR("Ack reports corrupted data, will retry: type",
                          static_cast<uint16_t>(ack.type), "target", static_cast<uint8_t>(canId.src));
                return;
            }
            pendingAcks.erase(pendingAcks.begin() + i);
            return;
        }
    }
}

uint16_t Communication::ComputeCrc(const uint8_t *data, size_t length) {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < length; i++) {
        crc ^= static_cast<uint16_t>(data[i]) << 8;
        for (uint8_t bit = 0; bit < 8; bit++) {
            crc = (crc & 0x8000) ? static_cast<uint16_t>((crc << 1) ^ 0x1021) : static_cast<uint16_t>(crc << 1);
        }
    }
    return crc;
}

ECanAckStatus Communication::VerifyAndStripCrc(const uint8_t *data, uint8_t &length) {
    if (length < CRC_LENGTH) {
        LOG_ERROR("AckRequired payload too short for CRC:", length);
        return ECanAckStatus::STATUS_ERROR;
    }

    uint8_t payloadLength = length - CRC_LENGTH;
    uint16_t receivedCrc = static_cast<uint16_t>(data[payloadLength])
                            | (static_cast<uint16_t>(data[payloadLength + 1]) << 8);
    uint16_t computedCrc = ComputeCrc(data, payloadLength);

    length = payloadLength;
    return (computedCrc == receivedCrc) ? ECanAckStatus::STATUS_OK : ECanAckStatus::STATUS_ERROR;
}

void Communication::ResendPending(PendingAck &pending) {
    CanId canId;
    canId.src = me;
    canId.dst = pending.target;
    canId.type = pending.type;
    canId.flags = ECanFlags::FLAG_NONE;

    if (pending.data.size() > MAX_CAN_DATA_LENGTH) {
        largeCanMessageHandler.SendLargeMessage(canId, pending.data.data(), static_cast<uint8_t>(pending.data.size()), true);
        return;
    }

    canId.flags = ECanFlags::FLAG_ACK_REQUIRED;
    CanMessage msg{};
    msg.id = canId.ToRaw();
    msg.dlc = static_cast<uint8_t>(pending.data.size());
    memcpy(msg.data, pending.data.data(), pending.data.size());

    if (xQueueSend(txQueue, &msg, pdMS_TO_TICKS(10)) != pdTRUE) {
        LOG_ERROR("TX: Queue full (retry)");
    }
}

void Communication::CheckPendingAcks() {
    uint32_t now = millis();
    for (size_t i = 0; i < pendingAcks.size();) {
        PendingAck &pending = pendingAcks[i];
        if (now - pending.sentAt < ACK_TIMEOUT_MS) {
            ++i;
            continue;
        }

        if (pending.retriesLeft == 0) {
            LOG_ERROR("Ack timeout, giving up: type", static_cast<uint16_t>(pending.type),
                      "target", static_cast<uint8_t>(pending.target));

            CanAckPayload failPayload{pending.type, ECanAckStatus::STATUS_ERROR};
            CanId ackId(pending.target, me, ECanMsgType::CAN_AIRRIDE_ACK, ECanFlags::FLAG_NONE);
            Notify(ackId, reinterpret_cast<const uint8_t *>(&failPayload), sizeof(failPayload));

            pendingAcks.erase(pendingAcks.begin() + i);
            continue;
        }

        LOG_WARN("Ack timeout, retrying: type", static_cast<uint16_t>(pending.type),
                 "target", static_cast<uint8_t>(pending.target), "retriesLeft", pending.retriesLeft);
        ResendPending(pending);
        pending.retriesLeft--;
        pending.sentAt = now;
        ++i;
    }
}
