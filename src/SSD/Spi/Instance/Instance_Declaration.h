/**
 * This file contains the SPI object instance declaration for the selected platform.
 */
#ifndef INSTANCE_DECLARATION_H
#define INSTANCE_DECLARATION_H

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__)

Spi_ARM Spi_;
  
// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

Spi_AVR Spi_;

// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

Spi_ESP8266 Spi_;

// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

Spi_Teensy_3x Spi_;
 
// ====================================
// Teensy LC:
// ====================================
#elif defined(__MKL26Z64__)

Spi_Teensy_LC Spi_;

// ====================================
// All other platforms:
// ====================================
#else

Spi_Legacy Spi_;

#endif

#endif
