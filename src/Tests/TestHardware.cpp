#include <stdint.h>
#include <Arduino.h>
#include "pins.h"
#include "TestHardware.h"
#include "Utils.h"

// My driver:
#ifdef USE_MINE

// Method to log errors initializing SPI.
static void log(const char* msg) {
    SerialPrintf("%s", msg);
}

void TestHardware::init(
    #ifdef TEENSY_3_1
    uint8_t nopCommand
    #endif
) {
    // Create SPI instance.
    SPI_Driver spi = SPI_Driver();
    spi.validatePins(SPI_PINS, log);

    #ifdef TEENSY_3_1
    spi.init(SPI_PINS, nopCommand);
    #else
    spi.init(SPI_PINS);
    #endif

    // Create OLED.
    ssdCore = new SSD_Core(spi, rstPin);

    // Create text and graphics engines.
    textEngine = new Text_Engine();
    textEngine->begin(ssdCore);
    graphicsEngine = new Graphics_Engine();
    graphicsEngine->begin(ssdCore);
}

// Original Sumotoy driver:
#else

void TestHardware::init() {
    ssd13XX = new SSD_13XX(SSD_INIT_ARGS);
    ssd13XX->begin(false);
}

#endif
