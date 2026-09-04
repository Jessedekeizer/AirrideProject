#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <functional>
#include <vector>
#include "CanID.h"
#include "LargeCanMessageHandler.h"
#include "freertos_include.h"

using Callback = std::function<void(const CanId &, const uint8_t *, uint8_t)>;

class Communication {
public:
    Communication(LargeCanMessageHandler &largeCanMessageHandler, ECanNode me);

    ~Communication();

    void SetQueues(QueueHandle_t rxQueue, QueueHandle_t txQueue);

    int Subscribe(Callback callback);

    void Unsubscribe(int id);

    void Notify(const CanId &canId, const uint8_t *data, uint8_t length);

    void CheckForMessage();

    template<typename T>
    void SendCanMessage(ECanNode target, ECanMsgType type, const T &messageStruct, bool requireAck = false) {
        SendCanMessage(target, type,
                       reinterpret_cast<const uint8_t *>(&messageStruct),
                       static_cast<uint8_t>(sizeof(T)), requireAck);
    }

private:
    struct Subscription {
        unsigned int id;
        Callback callback;
    };

    struct PendingAck {
        ECanNode target;
        ECanMsgType type;
        std::vector<uint8_t> data; // includes the trailing CRC16, as actually sent
        uint32_t sentAt;
        uint8_t retriesLeft;
    };

    static constexpr uint32_t ACK_TIMEOUT_MS = 600;
    static constexpr uint8_t MAX_RETRIES = 3;
    static constexpr uint8_t CRC_LENGTH = 2;

    void SendCanMessage(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length, bool requireAck = false);

    void SendAck(ECanNode target, ECanMsgType ackedType, ECanAckStatus status);

    void DecodeCanMessage(const CanMessage &message);

    void TrackPendingAck(ECanNode target, ECanMsgType type, const uint8_t *data, uint8_t length);

    void HandleAckReceived(const CanId &canId, const uint8_t *data, uint8_t length);

    void CheckPendingAcks();

    void ResendPending(PendingAck &pending);

    static uint16_t ComputeCrc(const uint8_t *data, size_t length);

    // Reads+strips the trailing CRC16 from a received AckRequired payload,
    // shrinking length to the real struct size. Returns whether it matched.
    static ECanAckStatus VerifyAndStripCrc(const uint8_t *data, uint8_t &length);

    std::vector<Subscription> subscribers;
    std::vector<PendingAck> pendingAcks;
    unsigned int nextId;
    QueueHandle_t rxQueue;
    QueueHandle_t txQueue;
    LargeCanMessageHandler &largeCanMessageHandler;
    const ECanNode me;
};

#endif // COMMUNICATION_H
