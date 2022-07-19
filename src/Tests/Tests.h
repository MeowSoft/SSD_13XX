#ifndef TESTS_H
#define TESTS_H

#include "TestConfig.h"
#include <stdint.h>

class Tests {

    public:

        void setup();

        void clearScreen();

        uint32_t testFillScreen(uint16_t color);
        uint32_t testDiagonalLines(uint16_t color, uint8_t corner);
        uint32_t testStraightLines(uint16_t color, bool horizontal);
        uint32_t testRectangles(uint16_t color);
        uint32_t testFilledRectangles(uint16_t color1, uint16_t color2);
        uint32_t testGradients(uint16_t color1, uint16_t color2);
};

#endif
