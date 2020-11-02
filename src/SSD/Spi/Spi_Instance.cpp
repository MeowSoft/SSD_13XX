#include "Spi_Instance.h"

// Teensy 3.x:
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

USE_NAMESPACE_SPI

Spi_Teensy_3x Spi_ = Spi_Teensy_3x();
  
static uint8_t initError;

void InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t cd,
    uint8_t nop,
    bool initSpi
) {
    Spi_.InitSpi(sdo, sck, cs, cd, nop, initSpi, &initError); 
}

// ARM (Due):
#elif defined(__SAM3X8E__)

USE_NAMESPACE_SPI

Spi_Due Spi_ = Spi_Due();
  
void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

// AVR:
#elif defined(__AVR__)

USE_NAMESPACE_SPI

Spi_AVR Spi_ = Spi_AVR();

void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

// Teensy LC:
#elif defined(__MKL26Z64__)

USE_NAMESPACE_SPI

Spi_Teensy_LC Spi_ = Spi_Teensy_LC();

uint8_t initError;

void InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t cd,
    uint8_t nop,
    bool initSpi
) {
    Spi_.InitSpi(sdo, sck, cs, cd, nop, initSpi, &initError); 
}

// ESP8266:
#elif defined(ESP8266)

USE_NAMESPACE_SPI

Spi_ESP8266 Spi_ = Spi_ESP8266();

void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

// All other platforms:
#else

USE_NAMESPACE_SPI

Spi_Legacy Spi_ = Spi_Legacy();

void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

#endif
