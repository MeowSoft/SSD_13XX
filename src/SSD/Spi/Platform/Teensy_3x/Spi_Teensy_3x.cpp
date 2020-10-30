#include "Spi_Teensy_3x.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#define MAX_SPI_SPEED ((uint32_t)30000000)
#define SPI_PINS_VALID(sdo, sck) ((sdo == 11 || sdo == 7) && (sck == 13 || sck == 14))

#ifdef SPI_NAMESPACE
using namespace SPI_NAMESPACE;
#endif

void Spi_Teensy_3x::InitSpi(SpiSetup initData) {

    // If SPI pins aren't valid, then set error and bail.
	if (!SPI_PINS_VALID(initData.sdo, initData.sck)){
        *initData.initSuccess = false;
		return;
    }

    // If CS pin is invalid, then bail.
    if (!SPI.pinIsChipSelect(initData.cd, initData.cs)) {
        *initData.initSuccess = false;
        return;
    }

    // Otherwise, continue with setup.
    spiSettings_ = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    SPI.setMOSI(initData.sdo);
    SPI.setSCK(initData.sck);
	if (initData.initSpi) SPI.begin();
    csDataMask_ = SPI.setCS(initData.cs);
    csCommandMask_ = csDataMask_ | SPI.setCS(initData.cd);

    *initData.initSuccess = true;
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_Teensy_3x::SetBitrate(uint32_t rate)
{
    // Nop
}
#endif

#endif
