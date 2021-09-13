#include "Spi_AVR.h"

// Only compile for AVR
#if defined(__AVR__)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)8000000)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_AVR::init(
    const uint8_t csPin,
    const uint8_t dcPin
) {
    #if defined(SPI_HAS_TRANSACTION)
    _spiSettings = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    pinMode(dcPin, OUTPUT);
	pinMode(csPin, OUTPUT);
	_csPort    = portOutputRegister(digitalPinToPort(csPin));
	_dcPort    = portOutputRegister(digitalPinToPort(dcPin));
	_csPinMask = digitalPinToBitMask(csPin);
	_dcPinMask = digitalPinToBitMask(dcPin);

    SPI.begin();

    #if !defined(SPI_HAS_TRANSACTION)
    SPI.setClockDivider(SPI_CLOCK_DIV2);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
    #endif

	*_csPort |= _csPinMask;
	_selectData();
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_AVR::setBitrate(uint32_t n) {
    if (n >= 8000000) {
        SPI.setClockDivider(SPI_CLOCK_DIV2);
    } else if (n >= 4000000) {
        SPI.setClockDivider(SPI_CLOCK_DIV4);
    } else if (n >= 2000000) {
        SPI.setClockDivider(SPI_CLOCK_DIV8);
    } else {
        SPI.setClockDivider(SPI_CLOCK_DIV16);
    }
}
#endif

#endif
