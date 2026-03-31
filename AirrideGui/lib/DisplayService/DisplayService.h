#ifndef DISPLAYSERVICE_H
#define DISPLAYSERVICE_H

#include <Arduino.h>
#include <FS.h>
#include <SPI.h>
#include <TFT_eSPI.h>
#include <PNGdec.h>

#include "SdCardService.h"

class DisplayService
{
public:
    explicit DisplayService(SdCardService &sdCardService);

    bool Begin();
    bool IsReady() const;

    void DrawText(const String &text, int x, int y, int font = 2, uint32_t foreground = TFT_WHITE, uint32_t background = TFT_BLACK);

    bool DrawImage(const char *path, int x = 0, int y = 0);
    bool DrawRectangle(int x = 0, int y = 0, int width = 0, int height = 0, uint32_t color = TFT_BLACK);

    void DrawPressure(float front, float back);
    void DrawBoolIcon(bool value, int x, int y);

    void PngDraw(PNGDRAW *pDraw);
    void *PngOpen(const char *filename, int32_t *size);
    void PngClose(void *handle);
    int32_t PngRead(PNGFILE *page, uint8_t *buffer, int32_t length);
    int32_t PngSeek(PNGFILE *page, int32_t position);

private:
    bool ValidateImageWidth(int32_t width) const;
    SdCardService &sdCardService;
    TFT_eSPI tft;
    PNG png;
    File pngFile;

    bool ready = false;
    int imageX = 0;
    int imageY = 0;
};

#endif
