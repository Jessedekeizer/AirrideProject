#include "DisplayService.h"
#include "Logger.h"

#define SCREEN_WIDTH TFT_HEIGHT //Screen is rotated 90 degrees

namespace {
    DisplayService *activeDisplay = nullptr;

    void PngDrawCallback(PNGDRAW *pDraw) {
        if (activeDisplay != nullptr) {
            activeDisplay->PngDraw(pDraw);
        }
    }

    void *PngOpenCallback(const char *filename, int32_t *size) {
        if (activeDisplay != nullptr) {
            return activeDisplay->PngOpen(filename, size);
        }
        *size = 0;
        return nullptr;
    }

    void PngCloseCallback(void *handle) {
        if (activeDisplay != nullptr) {
            activeDisplay->PngClose(handle);
        }
    }

    int32_t PngReadCallback(PNGFILE *page, uint8_t *buffer, int32_t length) {
        if (activeDisplay != nullptr) {
            return activeDisplay->PngRead(page, buffer, length);
        }
        return 0;
    }

    int32_t PngSeekCallback(PNGFILE *page, int32_t position) {
        if (activeDisplay != nullptr) {
            return activeDisplay->PngSeek(page, position);
        }
        return 0;
    }
}

DisplayService::DisplayService(SdCardService &sdCardService)
    : sdCardService(sdCardService) {
}

bool DisplayService::Begin() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    ready = true;
    return true;
}

bool DisplayService::IsReady() const {
    return ready;
}

void DisplayService::DrawText(const String &text, int x, int y, int font, uint32_t foreground, uint32_t background) {
    if (!ready) {
        return;
    }

    tft.setTextColor(foreground, background);
    tft.drawString(text, x, y, font);
}

void DisplayService::DrawPressure(float front, float back) {
    if (!ready) {
        return;
    }

    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.drawString(String(front, 1), 85, 140, 4);
    tft.drawString(String(back, 1), 201, 140, 4);
}

void DisplayService::DrawBoolIcon(bool value, int x, int y) {
    if (!ready) {
        return;
    }

    tft.fillRect(x + 1, y + 1, 33, 33, TFT_BLACK);

    if (sdCardService.IsReady()) {
        if (value) {
            if (!DrawImage("/Check.png", x + 4, y + 7)) {
                DrawText("Y", x + 10, y + 8, 2);
            }
        }
        else {
            if (!DrawImage("/Cross.png", x + 8, y + 8)) {
                DrawText("N", x + 10, y + 8, 2);
            }
        }
        return;
    }

    DrawText(value ? "Y" : "N", x + 10, y + 8, 2);
}

bool DisplayService::DrawImage(const char *path, int x, int y) {
    if (!ready) {
        LOG_ERROR("DisplayService::DrawImage - display not ready");
        return false;
    }

    if (!sdCardService.IsReady()) {
        LOG_ERROR("DisplayService::DrawImage - sd card not ready");
        return false;
    }

    File testFile = sdCardService.GetFileSystem().open(path);
    if (!testFile || testFile.isDirectory()) {
        LOG_ERROR(String("DisplayService::DrawImage - failed to open image: ") + path);
        return false;
    }

    testFile.close();

    imageX = x;
    imageY = y;
    activeDisplay = this;

    int16_t result = png.open(path, PngOpenCallback, PngCloseCallback, PngReadCallback, PngSeekCallback, PngDrawCallback);

    if (result != PNG_SUCCESS) {
        LOG_ERROR(String("DisplayService::DrawImage - png open failed: ") + path);
        activeDisplay = nullptr;
        return false;
    }

    if (!ValidateImageWidth(png.getWidth())) {
        LOG_ERROR(String("DisplayService::DrawImage - image too wide: ") + path);
        png.close();
        activeDisplay = nullptr;
        return false;
    }

    tft.startWrite();
    result = png.decode(nullptr, 0);
    tft.endWrite();
    png.close();

    activeDisplay = nullptr;

    if (result != PNG_SUCCESS) {
        LOG_ERROR(String("DisplayService::DrawImage - png decode failed: ") + path);
        return false;
    }

    return true;
}

bool DisplayService::DrawRectangle(int x, int y, int width, int height, uint32_t color) {
    if (!ready)
        return false;

    tft.fillRect(x, y, width, height, color);
    return true;
}

void DisplayService::PngDraw(PNGDRAW *pDraw) {
    uint16_t lineBuffer[SCREEN_WIDTH];
    png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
    tft.pushImage(imageX, imageY + pDraw->y, pDraw->iWidth, 1, lineBuffer);
}

void *DisplayService::PngOpen(const char *filename, int32_t *size) {
    pngFile = sdCardService.GetFileSystem().open(filename, FILE_READ);
    if (!pngFile) {
        *size = 0;
        return nullptr;
    }
    *size = pngFile.size();
    return &pngFile;
}

void DisplayService::PngClose(void *handle) {
    File *file = static_cast<File *>(handle);
    if (file != nullptr && *file) {
        file->close();
    }
}

int32_t DisplayService::PngRead(PNGFILE *page, uint8_t *buffer, int32_t length) {
    if (!pngFile) {
        return 0;
    }
    page = page;
    return pngFile.read(buffer, length);
}

int32_t DisplayService::PngSeek(PNGFILE *page, int32_t position) {
    if (!pngFile) {
        return 0;
    }
    page = page;
    return pngFile.seek(position) ? position : 0;
}

bool DisplayService::ValidateImageWidth(int32_t width) const {
    return width > 0 && width <= SCREEN_WIDTH;
}
