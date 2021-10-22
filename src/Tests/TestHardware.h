#ifndef HARDWARE_H
#define HARDWARE_H

#include "TestConfig.h"

// Convenience macro for Teensy 3.1.
#if defined(__MK20DX128__) || \
    defined(__MK20DX256__) || \
    defined(__MK64FX512__) || \
    defined(__MK66FX1M0__)
    #define TEENSY_3_1
#endif

// Library headers:
#ifdef USE_MINE
#include "SSD/SSD_Core.h"
#include "SSD/Text_Engine.h"
#include "SSD/Graphics_Engine.h"
#else
#include <SSD_13XX.h>
#endif

class TestHardware {

    public:

    // My driver:
    #ifdef USE_MINE

    // Hardware:
    SSD_Core* ssdCore;
    Text_Engine* textEngine;
    Graphics_Engine* graphicsEngine;

    // Init method:
    void init(
        #ifdef TEENSY_3_1
        uint8_t nopCommand
        #endif
    );

    // Original Sumotoy driver:
    #else

    // Hardware:
    SSD_13XX* ssd13XX;

    // Init method:
    void init();

    #endif
};

#endif
