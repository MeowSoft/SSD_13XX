#include "Tests.h"
#include "TestHardware.h"

#ifdef USE_MINE

// #include "_includes/_common_16bit_colors.h"
// #include "SSD/Text_Engine.h"
// #include "SSD/Graphics_Engine.h"
// #include "SSD/_fonts/Terminal_9.c"
// #include "SSD/SSD_Util.h"

#define _GetWidth testHardware.ssdCore->getScreenWidth
#define _GetHeight testHardware.ssdCore->getScreenHeight
#define _FillScreen testHardware.ssdCore->fillScreen
#define _DrawLine testHardware.ssdCore->drawLine
#define _DrawHLine(x, y, w, c) testHardware.ssdCore->drawLine(x, y, x + w, y, c)
#define _DrawVLine(x, y, h, c) testHardware.ssdCore->drawLine(x, y, x, y + h, c)
#define _DrawFilledRectangle(x, y, w, h, c1, c2) testHardware.ssdCore->drawRectangle(x, y, w, h, c1, c2, true)
// #define _DrawRectangle(x, y, w, h, c1) testHardware.ssdCore->drawRectangle(x, y, w, h, c1, c1, false)
// #define _SetRotation testHardware.ssdCore->setScreenRotation
// #define _ChangeMode testHardware.ssdCore->setScreenMode

// #define _SetCursor testHardware.textEngine->setCursor
// #define _SetTextColor testHardware.textEngine->setTextColor
// #define _SetTextScale testHardware.textEngine->setTextScale
// #define _PrintText testHardware.textEngine->println
// #define _SetFont testHardware.textEngine->setFont

// #define _DrawArc testHardware.graphicsEngine->drawArc
// #define _FillCircle testHardware.graphicsEngine->fillCircle
// #define _DrawCircle testHardware.graphicsEngine->drawCircle
// #define _DrawTriangle testHardware.graphicsEngine->drawTriangle
// #define _FillTriangle testHardware.graphicsEngine->fillTriangle
// #define _DrawRoundRect testHardware.graphicsEngine->drawRoundRect
// #define _FillRoundRect testHardware.graphicsEngine->fillRoundRect

// #define _Color565 SSD_Util::Color565

#else

// #include <SSD_13XX.h>
// #include <_fonts/Terminal_9.c>

#define _GetWidth testHardware.ssd13XX->width
#define _GetHeight testHardware.ssd13XX->height
#define _FillScreen testHardware.ssd13XX->fillScreen
#define _DrawLine testHardware.ssd13XX->drawLine
#define _DrawHLine testHardware.ssd13XX->drawFastHLine
#define _DrawVLine testHardware.ssd13XX->drawFastVLine
#define _DrawFilledRectangle(x, y, w, h, c1, c2) testHardware.ssd13XX->drawRect(x, y, w, h, c1, c2, true)
// #define _DrawRectangle testHardware.ssd13XX->drawRect
// #define _SetRotation testHardware.ssd13XX->setRotation
// #define _ChangeMode testHardware.ssd13XX->changeMode

// #define _SetCursor testHardware.ssd13XX->setCursor
// #define _SetTextColor testHardware.ssd13XX->setTextColor
// #define _SetTextScale testHardware.ssd13XX->setTextScale
// #define _PrintText testHardware.ssd13XX->println
// #define _SetFont testHardware.ssd13XX->setFont

// #define _DrawArc testHardware.ssd13XX->drawArc
// #define _FillCircle testHardware.ssd13XX->fillCircle
// #define _DrawCircle testHardware.ssd13XX->drawCircle
// #define _DrawTriangle testHardware.ssd13XX->drawTriangle
// #define _FillTriangle testHardware.ssd13XX->fillTriangle
// #define _DrawRoundRect testHardware.ssd13XX->drawRoundRect
// #define _FillRoundRect testHardware.ssd13XX->fillRoundRect

// #define _Color565 testHardware.ssd13XX->Color565

#endif

TestHardware testHardware = TestHardware();

void Tests::setup() {
    #if defined(USE_MINE) && defined(TEENSY_3_1)
    testHardware.init(0); //TODO
    #else
    testHardware.init();
    #endif
}

void Tests::clearScreen() {
    _FillScreen(0x0000);
}

uint32_t Tests::testFillScreen(uint16_t color) {

    uint32_t start;
    uint32_t result;
    
    _FillScreen(0x0000);
    start = micros();
    _FillScreen(color);
    result = micros() - start;
    return result;
}

uint32_t Tests::testDiagonalLines(uint16_t color, uint8_t corner) {

    // Time vars.
    uint32_t start;
    uint32_t result = 0;

    // Screen dims.
    int w = _GetWidth();
    int h = _GetHeight();

    // Line coordinates.
    int x1;
    int y1;
    int x2;
    int y2;

    // Loop index.
    int i;

    // Clear the screen.
    clearScreen();

    // Figure out which corner we want to start from.
    uint8_t _corner = corner % 4;

    // Run the test.
    if (_corner == 0) {

        x1 = 0;
        y1 = 0;
        y2 = h - 1;
        x2 = w - 1;

        start = micros();
        for (i = 0; i < w; i += 6) _DrawLine(x1, y1, i, y2, color);
        for (i = 0; i < h; i += 6) _DrawLine(x1, y1, x2, i, color);
        result += micros() - start;

    } else if (_corner == 1) {
   
        x1 = w - 1;
        y1 = 0;
        y2 = h - 1;
        x2 = 0;

        start = micros();
        for (i = 0; i < w; i += 6) _DrawLine(x1, y1, i, y2, color);
        for (i = 0; i < h; i += 6) _DrawLine(x1, y1, x2, i, color);
        result += micros() - start;

    } else if (_corner == 2) {

        x1 = 0;
        y1 = h - 1;
        y2 = 0;
        x2 = w - 1;

        start = micros();
        for (i = 0; i < w; i += 6) _DrawLine(x1, y1, i, y2, color);
        for (i = 0; i < h; i += 6) _DrawLine(x1, y1, x2, i, color);
        result += micros() - start;

    } else {

        x1 = w - 1;
        y1 = h - 1;
        y2 = 0;
        x2 = 0;

        start = micros();
        for (i = 0; i < w; i += 6) _DrawLine(x1, y1, i, y2, color);
        for (i = 0; i < h; i += 6) _DrawLine(x1, y1, x2, i, color);
        result += micros() - start;
    }

    // Return the result.
    return result;
}

uint32_t Tests::testStraightLines(uint16_t color, bool horizontal) {

    // Time vars.
    uint32_t start;
    uint32_t result = 0;

    // Screen dims.
    int w = _GetWidth();
    int h = _GetHeight();

    // Loop index.
    int i;

    // Clear the screen.
    clearScreen();

    // Run the test.
    start = micros();
    if (horizontal) {
        for (i = 0; i < h; i += 5) _DrawHLine(0, i, w, color);
    } else {
        for (i = 0; i < w; i += 5) _DrawVLine(i, 0, h, color);
    }
    result = micros() - start;

    // Return the result.
    return result;
}

uint32_t Tests::testFilledRectangles(uint16_t color1, uint16_t color2) {

    // Time vars.
    uint32_t start;
    uint32_t result = 0;

    int n;
    int i;
    int i2;
    int cx = (_GetWidth() / 2) - 1;
    int cy = (_GetHeight() / 2) - 1;

    // Clear the screen.
    clearScreen();

    n = min(_GetWidth(), _GetHeight());

    // Run the test.
    for (i = n; i > 0; i -= 10) {
        i2 = i / 2;
        start = micros();
        _DrawFilledRectangle(cx - i2, cy - i2, i, i, color1, color2);
        result += micros() - start;
    }

    // Return the result.
    return result;
}
