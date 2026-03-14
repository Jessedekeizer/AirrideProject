#include "CANOverSerial.h"
#include <HardwareSerial.h>

#include "Logger.h"

void CANOverSerial::SendMessage(CANMessage &message) {
    serial.write(START_BYTE);
    serial.write((uint8_t *) &message, sizeof(message));
    serial.write('\n');
}

bool CANOverSerial::ReceiveAvailable() {
    return serial.available();
}

void CANOverSerial::Receive() {
    while (serial.available()) {
        if (Serial2.read() == START_BYTE) {
            Serial2.readBytes((char *) &message, sizeof(message));

            char endByte = Serial2.read();

            if (endByte == '\n') {
                if (!canQueue.enqueue(message)) {
                    LOG_DEBUG("Receive: queue full");
                }
            }
        }
    }
}
