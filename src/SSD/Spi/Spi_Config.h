#ifndef SPI_CONFIG_H
#define SPI_CONFIG_H

#include "../Util.h"
#include <stdint.h>

// Namespace for SPI stuff.
#define NAMESPACE_SPI_NAME SpiNamespace

// Macros to turn SPI namespace on and off.
#ifdef NAMESPACE_SPI_NAME
#define NAMESPACE_SPI NAMESPACE_SPI_()
#define NAMESPACE_SPI_() __INLINE_EXPAND__(namespace NAMESPACE_SPI_NAME) {
#define NAMESPACE_SPI_END }
#define USE_NAMESPACE_SPI using namespace NAMESPACE_SPI_NAME;
#else 
#define NAMESPACE_SPI
#define NAMESPACE_SPI_END
#define USE_NAMESPACE_SPI
#endif

#endif
