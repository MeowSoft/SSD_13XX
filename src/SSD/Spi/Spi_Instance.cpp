#include "Spi_Instance.h"

USE_NAMESPACE_SPI

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__)

bool SPI_Instance::validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    return true;
}

void SPI_Instance::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
    Spi_ = Spi_Due();
    Spi_.init(csPin, dcPin); 
}

// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

bool SPI_Instance::validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    return true;
}

void SPI_Instance::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
    Spi_ = Spi_AVR();
    Spi_.init(csPin, cdcPind); 
}

// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

bool SPI_Instance::validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    return true;
}

void SPI_Instance::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
    Spi_ = Spi_ESP8266();
    Spi_.init(csPin, dcPin); 
}

// ====================================
// Teensy LC:
// ====================================
#elif defined(__MKL26Z64__)

bool SPI_Instance::validatePins(
    const uint8_t sdoPin,
    const uint8_t sckPin,
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    
    const char* error;
    if (!(Spi_.validatePins(sdoPin, sckPin, csPin, dcPin, error)) {
        logger(error);
        return false;
    }

    return true;
}

void SPI_Instance::init(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin
) {
    Spi_ = Spi_Teensy_LC();
    Spi_.init(sdoPin, sckPin, csPin, dcPin); 
}

// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

bool SPI_Instance::validatePins(
    const uint8_t sdoPin,
    const uint8_t sckPin,
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    const char* error = NULL;
    if (!Spi_.validatePins(sdoPin, sckPin, csPin, dcPin, error)) {
        logger(error);
        return false;
    }

    return true;
}

void SPI_Instance::init(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin,
    uint8_t nopCmd
) {
    Spi_ = Spi_Teensy_3x();
    Spi_.init(sdoPin, sckPin, csPin, dcPin, nopCmd); 
}

// ====================================
// All other platforms:
// ====================================
#else

bool SPI_Instance::validatePins(
    const uint8_t csPin,
    const uint8_t dcPin,
    LogMethod_t logger
) {
    return true;
}

void SPI_Instance::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
    Spi_ = Spi_Legacy();
    Spi_.init(csPin, dcPin); 
}

#endif
