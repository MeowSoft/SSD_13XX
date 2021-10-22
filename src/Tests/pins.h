#ifndef PINS_H
#define PINS_H

#include <stdint.h>

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__) 

    const uint8_t sckPin = 13;
    const uint8_t sdoPin = 11;
    const uint8_t csPin = 10;
    const uint8_t dcPin  = 8;
    const uint8_t rstPin = 9;

    #define SPI_PINS \
        csPin, \
        dcPin

    #define SSD_INIT_ARGS \
        csPin, \
        dcPin, \
        rstPin, \
        sdoPin, \
        sckPin
        
// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

    const uint8_t sckPin = 13;
    const uint8_t sdoPin = 11;
    const uint8_t csPin = 10;
    const uint8_t dcPin = 8;
    const uint8_t rstPin = 9;

    #define SPI_PINS \
        csPin, \
        dcPin

    #define SSD_INIT_ARGS \
        csPin, \
        dcPin, \
        rstPin, \
        sdoPin, \
        sckPin
        
// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

    const uint8_t sckPin = 13;
    const uint8_t sdoPin = 11;
    const uint8_t csPin = 15;
    const uint8_t dcPin = 0;
    const uint8_t rstPin = 2;

    #define SPI_PINS \
        csPin, \
        dcPin

    #define SSD_INIT_ARGS \
        csPin, \
        dcPin, \
        rstPin, \
        sdoPin, \
        sckPin
        
// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

    const uint8_t sckPin = 13;
    const uint8_t sdoPin = 11;
    const uint8_t csPin = 9;
    const uint8_t dcPin = 23;
    const uint8_t rstPin = 22;

    #define SPI_PINS \
        sdoPin, \
        sckPin, \
        csPin, \
        dcPin

    #define SSD_INIT_ARGS \
        csPin, \
        dcPin, \
        rstPin, \
        sdoPin, \
        sckPin
        
// ====================================
// Teensy LC:
// ====================================
#elif defined (__MKL26Z64__)

    const uint8_t sckPin = 13;
    const uint8_t sdoPin = 11;
    const uint8_t csPin = 9;
    const uint8_t dcPin = 23;
    const uint8_t rstPin = 22;

    #define SPI_PINS \
        sdoPin, \
        sckPin, \
        csPin, \
        dcPin

    #define SSD_INIT_ARGS \
        csPin, \
        dcPin, \
        rstPin, \
        sdoPin, \
        sckPin
        
#endif

#endif
