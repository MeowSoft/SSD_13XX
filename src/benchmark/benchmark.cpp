#include "benchmark.h"
#include "pins.h"

// OLED method aliases.
#ifdef USE_MINE
#define _GetWidth oled_->width
#define _GetHeight oled_->height
#define _FillScreen oled_->fillScreen
#define _DrawLine oled_->drawLine
#define _DrawHLine oled_->drawFastHLine
#define _DrawVLine oled_->drawFastVLine
#define _DrawFilledRectangle oled_->fillRect
#define _DrawRectangle oled_->drawRect
#define _SetRotation oled_->setRotation
#define _ChangeMode oled_->changeMode
#else
#define _GetWidth oled_->width
#define _GetHeight oled_->height
#define _FillScreen oled_->fillScreen
#define _DrawLine oled_->drawLine
#define _DrawHLine oled_->drawFastHLine
#define _DrawVLine oled_->drawFastVLine
#define _DrawFilledRectangle oled_->fillRect
#define _DrawRectangle oled_->drawRect
#define _SetRotation oled_->setRotation
#define _ChangeMode oled_->changeMode
#endif

// Private methods.
void testFillScreen(uint16_t color);
void testLines(uint16_t color);
void testFastLines(uint16_t color1, uint16_t color2);
void testFilledRects(uint16_t color1, uint16_t color2);

// The OLED.
SSD_13XX* oled_;

void benchmark_setup() {
    oled_ = new SSD_13XX(NEW_SSD_ARGS);
    oled_->begin(false);
}

void benchmark_run() {
    #ifdef USE_MINE
    SerialPrintf("Using library: mine\r\n");
    #else
    SerialPrintf("Using library: sumotoy\r\n");
    #endif

    for (uint8_t rotation = 0; rotation < 4; rotation++) {
        _FillScreen(0x0000);
        _SetRotation(
            #ifdef USE_MINE
            //(SSD_13xx_Rotation_Modes_t)rotation
            rotation
            #else
            rotation
            #endif
        );
        SerialPrintf("Benchmark: \r\n    rotation = %d\r\n", rotation);

        _ChangeMode(SSD_13XX_modes::NORMAL);
        delay(1000);
        testFillScreen(NAVY);
        delay(1000);
        testLines(0x07FF);
        delay(1000);
        testFastLines(RED, BLUE);
        delay(1000);
        testFilledRects(YELLOW, MAGENTA);
        delay(1000);
        _ChangeMode(SSD_13XX_modes::DISP_OFF);
        SerialPrintf("\r\n");
    }
}

void testFillScreen(uint16_t color) {

    uint32_t start;
    uint32_t t;
    
    _FillScreen(0x0000);
    start = micros();
    _FillScreen(color);
    t = micros() - start;

    SerialPrintf("    Fill screen: %d uSec\r\n", t);
}

void testLines(uint16_t color) {

    uint32_t start;
    uint32_t t;

    int x1;
    int y1;
    int x2;
    int y2;
    int w = _GetWidth();
    int h = _GetHeight();

    _FillScreen(0x0000);

    x1 = y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) _DrawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) _DrawLine(x1, y1, x2, y2, color);
    t = micros() - start;

    _FillScreen(0x0000);

    x1 = w - 1;
    y1 = 0;
    y2 = h - 1;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) _DrawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) _DrawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    _FillScreen(0x0000);

    x1 = 0;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) _DrawLine(x1, y1, x2, y2, color);
    x2 = w - 1;
    for (y2 = 0; y2 < h; y2 += 6) _DrawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    _FillScreen(0x0000);

    x1 = w - 1;
    y1 = h - 1;
    y2 = 0;
    start = micros();
    for (x2 = 0; x2 < w; x2 += 6) _DrawLine(x1, y1, x2, y2, color);
    x2 = 0;
    for (y2 = 0; y2 < h; y2 += 6) _DrawLine(x1, y1, x2, y2, color);
    t += micros() - start;

    SerialPrintf("    Lines: %d uSec\r\n", t);
}

void testFastLines(uint16_t color1, uint16_t color2) {

    uint32_t start;
    uint32_t t;

    int x;
    int y;
    int w = _GetWidth();
    int h = _GetHeight();

    _FillScreen(0x0000);

    start = micros();
    for (y = 0; y < h; y += 5) _DrawHLine(0, y, w, color1);
    for (x = 0; x < w; x += 5) _DrawVLine(x, 0, h, color2);
    t = micros() - start;

    SerialPrintf("    Fast lines: %d uSec\r\n", t);
}

void testFilledRects(uint16_t color1, uint16_t color2) {

    uint32_t start;
    uint32_t t = 0;

    int n;
    int i;
    int i2;
    int cx = (_GetWidth() / 2) - 1;
    int cy = (_GetHeight() / 2) - 1;

    _FillScreen(0x0000);

    n = min(_GetWidth(), _GetHeight());

    for (i = n; i > 0; i -= 6) {
        i2 = i / 2;
        start = micros();
        _DrawFilledRectangle(cx - i2, cy - i2, i, i, color1, color2);
        t += micros() - start;
    
        _DrawRectangle(cx - i2, cy - i2, i, i, color2);
    }

    SerialPrintf("    Filled rects: %d uSec\r\n", t);
}
