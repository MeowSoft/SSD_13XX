#ifndef PINS_H
#define PINS_H

#include <stdint.h>

// OLED pins for teensy 3.x:
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

    const uint8_t Pin_Sck = 13;
    const uint8_t Pin_Sdo = 11;
    const uint8_t Pin_CS = 9;
    const uint8_t Pin_Rst = 22;
    const uint8_t Pin_DC = 23;

    #define NEW_SSD_ARGS \
        Pin_CS, \
        Pin_DC, \
        Pin_Rst, \
        Pin_Sdo, \
        Pin_Sck

// OLED pins for DUE:
#elif defined(__SAM3X8E__) 

    const uint8_t Pin_CS = 10;
    const uint8_t Pin_Rst = 9;
    const uint8_t Pin_DC  = 8;

    #define NEW_SSD_ARGS \
        Pin_CS, \
        Pin_DC, \
        Pin_Rst

// OLED pins for Duemilanove:
#elif defined(__AVR__)

    const uint8_t Pin_CS = 10;
    const uint8_t Pin_Rst = 9;
    const uint8_t Pin_DC = 8;

    #define NEW_SSD_ARGS \
        Pin_CS, \
        Pin_DC, \
        Pin_Rst
        
// OLED pins for Teensy LC:
#elif defined (__MKL26Z64__)

    const uint8_t Pin_Sck = 13;
    const uint8_t Pin_Sdo = 11;
    const uint8_t Pin_CS = 9;
    const uint8_t Pin_Rst = 22;
    const uint8_t Pin_DC = 23;

    #define NEW_SSD_ARGS \
        Pin_CS, \
        Pin_DC, \
        Pin_Rst, \
        Pin_Sdo, \
        Pin_Sck

#endif
#endif
