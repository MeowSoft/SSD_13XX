#include "Spi_ESP8266.h"

// Only compile for ESP8266
#if defined(ESP8266)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)7999999)
#else
#define MAX_SPI_SPEED
#endif

USE_NAMESPACE_SPI

void Spi_ESP8266::InitSpi(
    const uint8_t cs,
    const uint8_t dc,
    bool initSpi
) {

    #if defined(SPI_HAS_TRANSACTION)
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    _dc = dc;
    _cs = cs;

    pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	if (initSpi) SPI.begin();
	#if !defined(SPI_HAS_TRANSACTION)
		if (initSpi){
			SPI.setClockDivider(4);
			SPI.setBitOrder(MSBFIRST);
			SPI.setDataMode(SPI_MODE0);
		}
	#endif
	#if !defined(ESP8266_FAST_CS)
		digitalWrite(_cs,HIGH);
	#else
		GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
	#endif
	enableDataStream();
}

#if !defined (SPI_HAS_TRANSACTION)
	void Spi_ESP8266::setBitrate(uint32_t n)
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
