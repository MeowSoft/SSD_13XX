#ifndef SPIMETHODS_H
#define SPIMETHODS_H

// Teensy 3.x:
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "Platform/Teensy_3x/Spi_Teensy_3x.h"

#ifdef SPI_NAMESPACE
using namespace SPI_NAMESPACE;
#endif

static Spi_Teensy_3x Spi_ = Spi_Teensy_3x();
  
inline void InitSpi(SpiSetup initData) {
    Spi_.InitSpi(initData); 
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

#endif
