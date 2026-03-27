#include "CANOverSerial.h"
#include <HardwareSerial.h>
#include "Logger.h"

void CANOverSerial::SendMessage(CanMessage &message) {
    serial.write(START_BYTE);
    serial.write((uint8_t *)&message, sizeof(message));
    serial.write('\n');
    LOG_DEBUG(
        "CAN msg id=", message.id,
        "dlc=", message.dlc,
        "data=",
        message.data[0],
        message.data[1],
        message.data[2],
        message.data[3],
        message.data[4], message.data[5],
        message.data[6],
        message.data[7]
    );
}

bool CANOverSerial::ReceiveAvailable() {
    return serial.available();
}

void CANOverSerial::Receive() {
    while (serial.available()) {
        if (serial.read() == START_BYTE) {
            serial.readBytes((char *)&message, sizeof(message));

            char endByte = serial.read();

            if (endByte == '\n') {
                if (!canQueue.enqueue(message)) {
                    LOG_DEBUG("Receive: queue full");
                }
            }
        }
    }
}
