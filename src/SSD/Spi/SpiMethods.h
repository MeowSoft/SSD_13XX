#ifndef SPIMETHODS_H
#define SPIMETHODS_H

// Teensy 3.x:
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "Platform/Teensy_3x/Spi_Teensy_3x.h"

USE_NAMESPACE_SPI

static Spi_Teensy_3x Spi_ = Spi_Teensy_3x();
  
static uint8_t initError;

inline void InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t cd,
    uint8_t nop,
    bool initSpi
) {
    Spi_.InitSpi(sdo, sck, cs, cd, nop, initSpi, &initError); 
}

#elif defined(__SAM3X8E__)

#include "Platform/Due/Spi_Due.h"

USE_NAMESPACE_SPI

static Spi_Due Spi_ = Spi_Due();
  
inline void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

#elif defined(__AVR__)

#include "Platform/AVR/Spi_AVR.h"

USE_NAMESPACE_SPI

static Spi_AVR Spi_ = Spi_AVR();

inline void InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {
    Spi_.InitSpi(cs, cd, initSpi); 
}

#elif defined(__MKL26Z64__)

#include "Platform/Teensy_LC/Spi_Teensy_LC.h"

USE_NAMESPACE_SPI

static Spi_Teensy_LC Spi_ = Spi_Teensy_LC();

static uint8_t initError;

inline void InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t cd,
    uint8_t nop,
    bool initSpi
) {
    Spi_.InitSpi(sdo, sck, cs, cd, nop, initSpi, &initError); 
}

#endif

inline void startTransaction(void) { Spi_.startTransaction(); }

inline void endTransaction(void) { Spi_.endTransaction(); }
          
inline void writecommand_cont(const uint8_t c) { Spi_.writecommand_cont(c); }

inline void writecommand16_cont(uint16_t c) { Spi_.writecommand16_cont(c); }

inline void writedata8_cont(uint8_t c) { Spi_.writedata8_cont(c); }

inline void writedata16_cont(uint16_t d) { Spi_.writedata16_cont(d); }

inline void writecommand_last(const uint8_t c) { Spi_.writecommand_last(c); }

inline void writedata8_last(uint8_t c) { Spi_.writedata8_last(c); }

inline void writedata16_last(uint16_t d) { Spi_.writedata16_last(d); }

inline void _pushColors_cont(uint16_t data,uint32_t times) { Spi_._pushColors_cont(data, times); }

inline void closeTransaction(void) { Spi_.closeTransaction(); }

#endif
