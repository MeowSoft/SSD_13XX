#include "Spi_Teensy_LC.h"

// Only compile for Teensy LC
#if defined(__MKL26Z64__)

#if defined(SPI_HAS_TRANSACTION)
#define MAX_SPI_SPEED ((uint32_t)8000000)
#else
#define MAX_SPI_SPEED
#endif

#define SPI0_PINS_VALID(sdo, sck) ((sdo == 11 || sdo == 7) && (sck == 13 || sck == 14))
#define SPI1_PINS_VALID(sdo, sck) ((sdo == 0 || sdo == 21) && (sck == 20))

USE_NAMESPACE_SPI

void Spi_Teensy_LC::InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t dc,
    uint8_t nop,
    bool initSpi,
    uint8_t* errorResult
) {
    // Init error result.
    *errorResult = 0;

    // If SPI pins aren't valid, then set error and bail.
	if (!SPI1_PINS_VALID(sdo, sck) && !SPI0_PINS_VALID(sdo, sck)){
		bitSet(*errorResult,0);
		return;
    }

    // Select SPI instance to use.
    _useSPI1 = SPI1_PINS_VALID(sdo, sck);
    SPIClass* spi_ = _useSPI1 ? &SPI1 : &SPI;

    // If cs pin is not valid, then set error and bail.
    if (!spi_->pinIsChipSelect(cs)) {
        bitSet(*errorResult, 1);
        return;
    }

    // Create SPI Settings.
    #if defined(SPI_HAS_TRANSACTION)
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    // Set cs and dc pin modes.
	pinMode(dc, OUTPUT);
	pinMode(cs, OUTPUT);

    // Set up SPI instance.
    spi_->setMOSI(sdo);
    spi_->setSCK(sck);
    if (initSpi) spi_->begin();
    
    // Set up vars for setting cs and dc signals.
	#if defined(TEENSY_LC_FAST_PORT)
		csportSet    	= portSetRegister(digitalPinToPort(cs));
		csportClear     = portClearRegister(digitalPinToPort(cs));
		cspinmask 		= digitalPinToBitMask(cs);
		dcportSet       = portSetRegister(digitalPinToPort(dc));
		dcportClear     = portClearRegister(digitalPinToPort(dc));
		dcpinmask	    = digitalPinToBitMask(dc);
		*csportSet 		= cspinmask;
	#else
        csPin = cs;
        dcPin = dc;
		digitalWriteFast(cs,HIGH);
	#endif
		
    enableDataStream();
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_Teensy_LC::setBitrate(uint32_t n)
{
    //nop
}
#endif

#endif
