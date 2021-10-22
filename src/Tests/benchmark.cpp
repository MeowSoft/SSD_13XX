




// // OLED method aliases.
// #ifdef USE_MINE

// #include "_includes/_common_16bit_colors.h"
// #include "SSD/Text_Engine.h"
// #include "SSD/Graphics_Engine.h"
// #include "SSD/_fonts/Terminal_9.c"
// #include "SSD/SSD_Util.h"

// #define _GetWidth _oled->getScreenWidth
// #define _GetHeight _oled->getScreenHeight
// #define _FillScreen _oled->fillScreen
// #define _DrawLine _oled->drawLine
// #define _DrawHLine(x, y, w, c) _oled->drawLine(x, y, x + w, y, c)
// #define _DrawVLine(x, y, h, c) _oled->drawLine(x, y, x, y + h, c)
// #define _DrawFilledRectangle(x, y, w, h, c1, c2) _oled->drawRectangle(x, y, w, h, c1, c2, true)
// #define _DrawRectangle(x, y, w, h, c1) _oled->drawRectangle(x, y, w, h, c1, c1, false)
// #define _SetRotation _oled->setScreenRotation
// #define _ChangeMode _oled->setScreenMode

// #define _SetCursor _texter->setCursor
// #define _SetTextColor _texter->setTextColor
// #define _SetTextScale _texter->setTextScale
// #define _PrintText _texter->println
// #define _SetFont _texter->setFont

// #define _DrawArc _graphics->drawArc
// #define _FillCircle _graphics->fillCircle
// #define _DrawCircle _graphics->drawCircle
// #define _DrawTriangle _graphics->drawTriangle
// #define _FillTriangle _graphics->fillTriangle
// #define _DrawRoundRect _graphics->drawRoundRect
// #define _FillRoundRect _graphics->fillRoundRect

// #define _Color565 SSD_Util::Color565

// #else

// #include <_fonts/Terminal_9.c>

// #define _GetWidth _oled->width
// #define _GetHeight _oled->height
// #define _FillScreen _oled->fillScreen
// #define _DrawLine _oled->drawLine
// #define _DrawHLine _oled->drawFastHLine
// #define _DrawVLine _oled->drawFastVLine
// #define _DrawFilledRectangle(x, y, w, h, c1, c2) _oled->drawRect(x, y, w, h, c1, c2, true)
// #define _DrawRectangle _oled->drawRect
// #define _SetRotation _oled->setRotation
// #define _ChangeMode _oled->changeMode

// #define _SetCursor _oled->setCursor
// #define _SetTextColor _oled->setTextColor
// #define _SetTextScale _oled->setTextScale
// #define _PrintText _oled->println
// #define _SetFont _oled->setFont

// #define _DrawArc _oled->drawArc
// #define _FillCircle _oled->fillCircle
// #define _DrawCircle _oled->drawCircle
// #define _DrawTriangle _oled->drawTriangle
// #define _FillTriangle _oled->fillTriangle
// #define _DrawRoundRect _oled->drawRoundRect
// #define _FillRoundRect _oled->fillRoundRect

// #define _Color565 _oled->Color565

// #endif

// // Private methods.
// void testFillScreen(uint16_t color);
// void testLines(uint16_t color);
// void testFastLines(uint16_t color1, uint16_t color2);
// void testFilledRects(uint16_t color1, uint16_t color2);
// void testText();
// void testText2();
// void testArc(uint16_t color);
// void testFilledCircles(uint8_t radius, uint16_t color);
// void testCircles(uint8_t radius, uint16_t color);
// void testTriangles();
// void testFilledTriangles();
// void testRoundRects();
// void testFilledRoundRects();

// #ifdef USE_MINE
// SSD_Core* _oled;
// Text_Engine* _texter;
// Graphics_Engine* _graphics;
// #else
// SSD_13XX* _oled;
// #endif

// void log(const char* msg) {
//     SerialPrintf("%s", msg);
// }

// void benchmark_setup() {

//     #ifdef USE_MINE

//     // Create SPI instance.
//     SPI_Driver spi = SPI_Driver();
//     spi.validatePins(SPI_PINS, log);
//     spi.init(SPI_INIT_ARGS);

//     // Create OLED.
//     _oled = new SSD_Core(spi, rstPin);

//     // Create text and graphics engines.
//     _texter = new Text_Engine();
//     _texter->begin(_oled);
//     _graphics = new Graphics_Engine();
//     _graphics->begin(_oled);

//     #else
//     _oled = new SSD_13XX(csPin, dcPin, rstPin, sdoPin, sckPin);
//     _oled->begin(false);
//     #endif
// }

// void benchmark_run2() {
//     #ifdef USE_MINE
//     SerialPrintf("Using library: mine\r\n");
//     #else
//     SerialPrintf("Using library: sumotoy\r\n");
//     #endif

//     _FillScreen(0x0000);
//     #ifdef USE_MINE
//     _ChangeMode(SSD_ScreenConfig::NORMAL);
//     #else
//     _ChangeMode(SSD_13XX_modes::NORMAL);
//     #endif
//     delay(1000);
//     testFilledRects(YELLOW, MAGENTA);
// }

// void benchmark_run() {
//     #ifdef USE_MINE
//     SerialPrintf("Using library: mine\r\n");
//     #else
//     SerialPrintf("Using library: sumotoy\r\n");
//     #endif

//     for (uint8_t rotation = 0; rotation < 5; rotation++) {
//         _FillScreen(0x0000);
//         _SetRotation(
//             #ifdef USE_MINE
//             //(SSD_13xx_Rotation_Modes_t)rotation
//             (SSD_ScreenConfig::Rotations)(rotation % 4)
//             #else
//             rotation
//             #endif
//         );
//         SerialPrintf("Benchmark: \r\n    rotation = %d\r\n", (rotation % 4));

//         #ifdef USE_MINE
//         _ChangeMode(SSD_ScreenConfig::NORMAL);
//         #else
//         _ChangeMode(SSD_13XX_modes::NORMAL);
//         #endif
//         delay(1000);

//         testFillScreen(NAVY);
//         delay(1000);
//         testLines(0x07FF);
//         delay(1000);
//         testFastLines(RED, BLUE);
//         delay(1000);
//         testFilledRects(YELLOW, MAGENTA);
//         delay(1000);
//         testText();
//         delay(1000);
//         testText2();
//         delay(1000);
//         testArc(CYAN);
//         delay(1000);
//         testFilledCircles(5, MAGENTA);
//         delay(1000);
//         testCircles(5, WHITE);
//         delay(1000);
//         testTriangles();
//         delay(1000);
//         testFilledTriangles();
//         delay(1000);
//         testRoundRects();
//         delay(1000);
//         testFilledRoundRects();
//         delay(1000);

//         #ifdef USE_MINE
//         _ChangeMode(SSD_ScreenConfig::DISP_OFF);
//         #else
//         _ChangeMode(SSD_13XX_modes::DISP_OFF);
//         #endif
//         SerialPrintf("\r\n");
//     }
// }

// /* #region Test methods: */







// void testFilledRects(uint16_t color1, uint16_t color2) {

//     uint32_t start;
//     uint32_t t = 0;

//     int n;
//     int i;
//     int i2;
//     int cx = (_GetWidth() / 2) - 1;
//     int cy = (_GetHeight() / 2) - 1;

//     _FillScreen(0x0000);

//     n = min(_GetWidth(), _GetHeight());

//     for (i = n; i > 0; i -= 10) {
//         i2 = i / 2;
//         start = micros();
//         _DrawFilledRectangle(cx - i2, cy - i2, i, i, color1, color2);
//         t += micros() - start;
//     }

//     SerialPrintf("    Filled rects: %d uSec\r\n", t);
// }

// void testText() {
//     _FillScreen(0x0000);
//     unsigned long start = micros();
//     _SetCursor(0, 0);
//     _SetTextColor(WHITE);
//     _SetTextScale(1);
//     _PrintText("Hello World!");
//     _SetTextColor(YELLOW);
//     _SetTextScale(2);
//     #if (SSD_HEIGHT > 64)
//     _PrintText(1234.56);
//     #else
//     _PrintText(1234.5);
//     #endif
//     _SetTextColor(RED);
//     #if (SSD_HEIGHT > 64)
//     _SetTextScale(3);
//     #else
//     _SetTextScale(2);
//     #endif
//     _PrintText(0xDEAD, HEX);
//     #if (SSD_HEIGHT > 64)
//     _PrintText();
//     _SetTextColor(GREEN);
//     _SetTextScale(4);
//     _PrintText("Hello");
//     #endif
//     unsigned long t = micros() - start;
//     SerialPrintf("    Text 1: %d uSec\r\n", t);
// }

// void testText2() {
//     _FillScreen(0x0000);
//     _SetFont(&Terminal_9);
//     unsigned long start = micros();
//     _SetCursor(0, 0);
//     _SetTextColor(WHITE);
//     _SetTextScale(2);
//     _PrintText("I implore thee,");
//     _SetTextScale(1);
//     _PrintText("my foonting turlingdromes.");
//     _PrintText("And hooptiously drangle me");
//     _PrintText("with crinkly bindlewurdles,");
//     _PrintText("Or I will rend thee");
//     _PrintText("in the gobberwarts");
//     _PrintText("with my blurglecruncheon,");
//     _PrintText("see if I don't!");
//     unsigned long t =  micros() - start;
//     SerialPrintf("    Text 2: %d uSec\r\n", t);
// }

// void testArc(uint16_t color) {
//     unsigned long start;
//     uint16_t i;
//     uint16_t cx = _GetWidth()  / 2;
//     uint16_t cy = _GetHeight() / 2;

//     _FillScreen(0x0000);

//     start = micros();
//     for (i = 0; i < 360; i += 5) {
//         _DrawArc(cx, cy, 30, 2, 0, i, color);
//     }
//     unsigned long t = micros() - start;
//     SerialPrintf("    Arcs: %d uSec\r\n", t);
// }

// void testFilledCircles(uint8_t radius, uint16_t color) {
//     unsigned long start;
//     int x;
//     int y;
//     int w = _GetWidth();
//     int h = _GetHeight();
//     int r2 = radius * 2;

//     _FillScreen(0x0000);

//     start = micros();
//     for (x = radius; x < w; x += r2) {
//         for (y = radius; y < h; y += r2) {
//             _FillCircle(x, y, radius, color);
//         }
//     }

//     unsigned long t = micros() - start;
//     SerialPrintf("    Filled circles: %d uSec\r\n", t);
// }

// void testCircles(uint8_t radius, uint16_t color) {
//     unsigned long start;
//     int x;
//     int y;
//     int r2 = radius * 2;
//     int w = _GetWidth() + radius;
//     int h = _GetHeight() + radius;

//     // Screen is not cleared for this one -- this is
//     // intentional and does not affect the reported time.
//     start = micros();
//     for (x = 0; x < w; x += r2) {
//         for (y = 0; y < h; y += r2) {
//             _DrawCircle(x, y, radius, color);
//         }
//     }

//     unsigned long t = micros() - start;
//     SerialPrintf("    Circles: %d uSec\r\n", t);
// }

// void testTriangles() {
//     unsigned long start;
//     int n;
//     int i;
//     int cx = _GetWidth()  / 2 - 1;
//     int cy = (_GetHeight() / 2) - 1;

//     _FillScreen(0x0000);

//     n = min(cx, cy);
//     start = micros();
//     for (i = 0; i < n; i += 5) {
//         _DrawTriangle(
//             cx    , cy - i, // peak
//             cx - i, cy + i, // bottom left
//             cx + i, cy + i, // bottom right
//             _Color565(0, 0, i)
//         );
//     }

//     unsigned long t = micros() - start;
//     SerialPrintf("    Triangles: %d uSec\r\n", t);
// }

// void testFilledTriangles() {
//     unsigned long start;
//     unsigned long t = 0;
//     int i;
//     int cx = (_GetWidth() / 2) - 1;
//     int cy = _GetHeight() / 2 - 1;

//     _FillScreen(0x0000);

//     start = micros();
//     for (i = min(cx, cy); i > 10; i -= 5) {
//         start = micros();
//         _FillTriangle(
//             cx, 
//             cy - i, 
//             cx - i, 
//             cy + i, 
//             cx + i, 
//             cy + i,
//             _Color565(0, i, i)
//         );
//         t += micros() - start;
//         _DrawTriangle(
//             cx, 
//             cy - i, 
//             cx - i, 
//             cy + i, 
//             cx + i, 
//             cy + i,
//             _Color565(i, i, 0)
//         );
//     }

//     SerialPrintf("    Filled triangles: %d uSec\r\n", t);
// }

// void testRoundRects() {
//     unsigned long start;
//     int w;
//     int i;
//     int i2;
//     int cx = (_GetWidth()  / 2) - 1;
//     int cy = (_GetHeight() / 2) - 1;

//     _FillScreen(0x0000);

//     w = min(_GetWidth(), _GetHeight());
//     start = micros();
//     for (i = 0; i < w; i += 6) {
//         i2 = i / 2;
//         _DrawRoundRect(
//             cx - i2, 
//             cy - i2, 
//             i, 
//             i, 
//             i / 8, 
//             _Color565(i, 0, 0)
//         );
//     }

//     unsigned long t = micros() - start;
//     SerialPrintf("    Rounded rects: %d uSec\r\n", t);
// }

// void testFilledRoundRects() {
//     unsigned long start;
//     int i;
//     int i2;
//     int cx = (_GetWidth()  / 2) - 1;
//     int cy = (_GetHeight() / 2) - 1;

//     _FillScreen(0x0000);

//     start = micros();
//     for (i = min(_GetWidth(), _GetHeight()); i > 20; i -= 6) {
//         i2 = i / 2;
//         _FillRoundRect(
//             cx - i2, 
//             cy - i2, 
//             i, 
//             i, 
//             i / 8, 
//             _Color565(0, i, 0)
//         );
//     }

//     unsigned long t = micros() - start;
//     SerialPrintf("    Filled rounded rects: %d uSec\r\n", t);
// }

// /* #endregion */
