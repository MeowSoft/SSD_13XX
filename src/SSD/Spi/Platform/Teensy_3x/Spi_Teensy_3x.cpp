#include "Spi_Teensy_3x.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

// Teensy max SPI speed.
#define MAX_SPI_SPEED ((uint32_t)30000000)

// Valid sdo and sck pins for teensy hardware SPI.
#define SPI_PINS_VALID(sdo, sck) ((sdo == 11 || sdo == 7) && (sck == 13 || sck == 14))

USE_NAMESPACE_SPI

void Spi_Teensy_3x::InitSpi(
    const uint8_t sdo,
    const uint8_t sck,
    const uint8_t cs,
    const uint8_t cd,
    uint8_t nop,
    bool initSpi,
    uint8_t* errorResult
) {
    // Init error result.
    *errorResult = 0;

    // If SPI pins aren't valid, then set error and bail.
	if (!SPI_PINS_VALID(sdo, sck)){
        bitSet(*errorResult,0);
		return;
    }

    // If CS pin is invalid, then bail.
    if (!SPI.pinIsChipSelect(cd, cs)) {
        bitSet(*errorResult, 1);
        return;
    }

    // Otherwise, continue with setup.
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    SPI.setMOSI(sdo);
    SPI.setSCK(sck);
	if (initSpi) SPI.begin();
    csDataMask_ = SPI.setCS(cs);
    csCommandMask_ = csDataMask_ | SPI.setCS(cd);
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_Teensy_3x::SetBitrate(uint32_t rate)
{
    // Nop
}
#endif

#endif
