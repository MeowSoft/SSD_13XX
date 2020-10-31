#include "Spi_AVR.h"

// Only compile for AVR
#if defined(__AVR__)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)8000000)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_AVR::InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {

    #if defined(SPI_HAS_TRANSACTION)
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    pinMode(cd, OUTPUT);
	pinMode(cs, OUTPUT);
	csport_    = portOutputRegister(digitalPinToPort(cs));
	rsport_    = portOutputRegister(digitalPinToPort(cd));
	cspinmask_ = digitalPinToBitMask(cs);
	dcpinmask_ = digitalPinToBitMask(cd);
    if (initSpi) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (initSpi){
			SPI.setClockDivider(SPI_CLOCK_DIV2); // 8 MHz
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	*csport_ |= cspinmask_;//hi
	enableDataStream();
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_AVR::setBitrate(uint32_t n)
{
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
