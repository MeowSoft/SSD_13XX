#include "Spi_Teensy_3x.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

// Teensy max SPI speed.
#define MAX_SPI_SPEED ((uint32_t)30000000)

// Valid sdo and sck pins for teensy hardware SPI.
#define SPI_PINS_VALID(sdo, sck) ((sdo == 11 || sdo == 7) && (sck == 13 || sck == 14))

USE_NAMESPACE_SPI

bool Spi_Teensy_3x::validatePins(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin,
    const char* errorMessage
) {
    if (!SPI_PINS_VALID(sdoPin, sckPin)) {
        errorMessage = "ERROR: sdo or sck are not valid SPI pins";
        return false;
    }

    if (!SPI.pinIsChipSelect(csPin, dcPin)) {
        errorMessage = "ERROR: cs or cd are not valid SPI pins";
        return false;
    }

    return true;
}
        
void Spi_Teensy_3x::init(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin,
    uint8_t nopCmd
) {
    // Set nop command to send when flushing the transmit queue.
    _nopCmd = nopCmd;

    // Set up SPI.
    _spiSettings = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    SPI.setMOSI(sdoPin);
    SPI.setSCK(sckPin);
	SPI.begin();
    
    // Get CS and D/C masks.
    _csDataMask = SPI.setCS(csPin);
    _csCommandMask = _csDataMask | SPI.setCS(dcPin);
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_Teensy_3x::SetBitrate(uint32_t rate) {
    // Nop
}
#endif

#endif
