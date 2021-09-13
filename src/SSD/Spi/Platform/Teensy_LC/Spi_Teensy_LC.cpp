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

bool Spi_Teensy_LC::validatePins(
    const uint8_t sdoPin, 
    const uint8_t sckPin, 
    const uint8_t csPin,
    const uint8_t dcPin,
    const char* errorMessage
) {
    if (!SPI1_PINS_VALID(sdoPin, sckPin) && !SPI0_PINS_VALID(sdoPin, sckPin)) {
        errorMessage = "ERROR: sdo or sck are not valid SPI pins";
        return false;
    }

    if (!SPI.pinIsChipSelect(csPin, dcPin)) {
        errorMessage = "ERROR: cs is not a valid SPI pin";
        return false;
    }

    return true;
}
       
void Spi_Teensy_LC::init(
    const uint8_t sdoPin,
    const uint8_t sckPin,
    const uint8_t csPin,
    const uint8_t dcPin
) {
    // Select SPI instance to use.
    _useSPI1 = SPI1_PINS_VALID(sdoPin, sckPin);
    SPIClass* spi = _useSPI1 ? &SPI1 : &SPI;

    // Create SPI Settings.
    #if defined(SPI_HAS_TRANSACTION)
    _spiSettings = SPISettings(MAX_SPI_SPEED, MSBFIRST, SPI_MODE0);
    #endif

    // Set cs and dc pin modes.
	pinMode(dcPin, OUTPUT);
	pinMode(csPin, OUTPUT);

    // Set up SPI instance.
    spi->setMOSI(sdoPin);
    spi->setSCK(sckPin);
    spi->begin();
    
    // Set up vars for setting cs and dc signals.
	#if defined(TEENSY_LC_FAST_PORT)
    _csPortSet = portSetRegister(digitalPinToPort(csPin));
    _csPortClear = portClearRegister(digitalPinToPort(csPin));
    _csPinMask = digitalPinToBitMask(csPin);
    _dcPortSet = portSetRegister(digitalPinToPort(dcPin));
    _dcPortClear = portClearRegister(digitalPinToPort(dcPin));
    _dcPinMask = digitalPinToBitMask(dcPin);
    *_csPortSet = _csPinMask;
	#else
    _csPin = csPin;
    _dcPin = dcPin;
    digitalWriteFast(_csPin, HIGH);
	#endif
		
    _selectData();
}

#if !defined (SPI_HAS_TRANSACTION)
void Spi_Teensy_LC::setBitrate(uint32_t n) {
    //nop
}
#endif

#endif
