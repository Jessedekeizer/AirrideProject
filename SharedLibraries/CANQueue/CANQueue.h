#ifndef STRINGQUEUE_H
#define STRINGQUEUE_H

#define QUEUE_SIZE 10

#include "CANMessage.h"

struct CANQueue {
    CANMessage messages[QUEUE_SIZE];
    uint8_t head = 0;
    uint8_t tail = 0;
    uint8_t count = 0;

    bool enqueue(const CANMessage &msg) {
        if (count >= QUEUE_SIZE) return false;

        messages[head] = msg;
        head = (head + 1) % QUEUE_SIZE;
        count++;
        return true;
    }

    bool dequeue(CANMessage &out) {
        if (count == 0) return false;

        out = messages[tail];
        tail = (tail + 1) % QUEUE_SIZE;
        count--;
        return true;
    }
};

#endif //STRINGQUEUE_H
