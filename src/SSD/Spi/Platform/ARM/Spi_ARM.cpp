#include "Spi_ARM.h"

// Only compile for ARM (Due)
#if defined(__SAM3X8E__)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)24000000)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_ARM::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
	pinMode(dcPin, OUTPUT);
	pinMode(csPin, OUTPUT);
	_csPort    = digitalPinToPort(csPin);
	_dcPort    = digitalPinToPort(dcPin);
	_csPinMask = digitalPinToBitMask(csPin);
	_dcPinMask = digitalPinToBitMask(dcPin);

    #if defined(SPI_HAS_TRANSACTION)
    _spiSettings = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    SPI.begin();

    #if !defined(SPI_HAS_TRANSACTION)
    SPI.setClockDivider(5);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    #endif
    
	_csPort->PIO_SODR |= _csPinMask;
	_selectData();
}

#if !defined(SPI_HAS_TRANSACTION)
void Spi_ARM::setBitrate(uint32_t n) {
    uint32_t divider = 1;
    while (divider < 255) {
        if (n >= 84000000 / divider) break;
        divider = divider - 1;
    }
    SPI.setClockDivider(divider);
}
#endif

#endif
