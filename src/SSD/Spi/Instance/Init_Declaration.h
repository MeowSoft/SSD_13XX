/**
 * This file contains the init method declaration for the selected platform.
 */
#ifndef INIT_DECLARATION_H
#define INIT_DECLARATION_H

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__)

void init(
    const uint8_t csPin,
    const uint8_t dcPin
);

// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

void init(
    const uint8_t csPin,
    const uint8_t dcPin
);

// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

void init(
    const uint8_t csPin,
    const uint8_t dcPin
);

// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

void init(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin,
    uint8_t nopCmd
);

// ====================================
// Teensy LC:
// ====================================
#elif defined(__MKL26Z64__)

void init(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin
);

// ====================================
// All other platforms:
// ====================================
#else

void init(
    const uint8_t csPin,
    const uint8_t dcPin
);

#endif

#endif
