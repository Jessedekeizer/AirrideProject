#ifndef SDCARDSERVICE_H
#define SDCARDSERVICE_H

#include <SD.h>
#include <SPI.h>

class SdCardService
{
public:
    SdCardService(uint8_t csPin, SPIClass& spi)
        : csPin(csPin), spi(spi), ready(false)
    {
    }

    bool Begin()
    {
        ready = SD.begin(csPin, spi, 80000000);
        return ready;
    }

    bool IsReady() const
    {
        return ready;
    }

    fs::FS& GetFileSystem()
    {
        return SD;
    }

private:
    uint8_t csPin;
    SPIClass& spi;
    bool ready;
};

#endif //SDCARDSERVICE_H