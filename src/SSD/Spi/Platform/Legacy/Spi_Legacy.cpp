#include "Spi_Legacy.h"

// Don't compile if the platform is implemented elsewhere.
#if defined(PLATFORM_LEGACY)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)8000000)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_Legacy::InitSpi(
    const uint8_t cs,
    const uint8_t dc,
    bool initSpi
) {

    #if defined(SPI_HAS_TRANSACTION)
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    _cs = cs;
    _dc = dc;

    pinMode(dc, OUTPUT);
	pinMode(cs, OUTPUT);
	if (initSpi) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (initSpi){
			SPI.setClockDivider(4);
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	digitalWrite(cs,HIGH);
	enableDataStream();
}

#if !defined (SPI_HAS_TRANSACTION)
	void Spi_Legacy::setBitrate(uint32_t n)
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
