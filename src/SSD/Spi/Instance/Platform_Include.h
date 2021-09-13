/**
 * This file contains the implementation include for the selected platform.
 */
#ifndef PLATFORM_INCLUDE_H
#define PLATFORM_INCLUDE_H

// ====================================
// ARM (Due):
// ====================================
#if defined(__SAM3X8E__)

#include "../Platform/ARM/Spi_ARM.h"

// ====================================
// AVR (Duemilanove):
// ====================================
#elif defined(__AVR__)

#include "../Platform/AVR/Spi_AVR.h"

// ====================================
// ESP8266:
// ====================================
#elif defined(ESP8266)

#include "../Platform/ESP8266/Spi_ESP8266.h"

// ====================================
// Teensy 3.x:
// ====================================
#elif defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#include "../Platform/Teensy_3x/Spi_Teensy_3x.h"

// ====================================
// Teensy LC:
// ====================================
#elif defined(__MKL26Z64__)

#include "../Platform/Teensy_LC/Spi_Teensy_LC.h"

// ====================================
// All other platforms:
// ====================================
#else

#include "../Platform/Legacy/Spi_Legacy.h"

#endif

#endif
