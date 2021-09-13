/**
 * This file contains the validatePins method declaration for the selected platform.
 */
#ifndef VALIDATEPINS_DECLARATION_H
#define VALIDATEPINS_DECLARATION_H

typedef void (*LogMethod_t)(const char*);

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__)

bool validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

bool validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

bool validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

bool validatePins(
    const uint8_t sdoPin,
    const uint8_t sckPin,
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

// ====================================
// Teensy LC:
// ====================================
#elif defined(__MKL26Z64__)

bool validatePins(
    const uint8_t sdoPin,
    const uint8_t sckPin,
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

// ====================================
// All other platforms:
// ====================================
#else

bool validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
);

#endif

#endif
