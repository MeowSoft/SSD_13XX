#include "Spi_Due.h"

// Only compile for ARM (Due)
#ifdef __SAM3X8E__

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)24000000)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_Due::InitSpi(
    const uint8_t cs,
    const uint8_t cd,
    bool initSpi
) {

	pinMode(cd, OUTPUT);
	pinMode(cs, OUTPUT);
	csport_    = digitalPinToPort(cs);
	rsport_    = digitalPinToPort(cd);
	cspinmask_ = digitalPinToBitMask(cs);
	dcpinmask_ = digitalPinToBitMask(cd);

    #if defined(SPI_HAS_TRANSACTION)
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    if (initSpi) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (initSpi){
			SPI.setClockDivider(5); // 8 MHz
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	csport_->PIO_SODR  |=  cspinmask_;//HI
	enableDataStream();
}

#if !defined(SPI_HAS_TRANSACTION)
	void Spi_Due::setBitrate(uint32_t n)
	{
		uint32_t divider = 1;
		while (divider < 255) {
			if (n >= 84000000 / divider) break;
			divider = divider - 1;
		}
		SPI.setClockDivider(divider);
	}
	#endif

#endif
