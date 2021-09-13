#include "Spi_Teensy_LC.h"

#define PREFIX inline

PREFIX void Spi_Teensy_LC::startTransaction(void) {
    if (_useSPI1){
        SPI1.beginTransaction(_spiSettings);
    } else {
        SPI.beginTransaction(_spiSettings);
    }

    #if !defined(TEENSY_LC_FAST_PORT)
    digitalWriteFast(_csPin, LOW);
    #else
    *_csPortClear = _csPinMask;
    #endif
}

PREFIX void Spi_Teensy_LC::endTransaction(void) {
    if (_useSPI1){
        SPI1.endTransaction();
    } else {
        SPI.endTransaction();
    }
}

PREFIX void Spi_Teensy_LC::writeData16Multi(
    uint16_t data,
    uint32_t times
) {
    _selectData();
    while (times--) {
        if (_useSPI1){
            SPI1.transfer16(data);
        } else {
            SPI.transfer16(data);
        }
    }
}

PREFIX void Spi_Teensy_LC::_selectData(void) {
    #if !defined(TEENSY_LC_FAST_PORT)
    digitalWriteFast(_dcPin, HIGH);
    #else
    *_dcPortSet = _dcPinMask;
    #endif
}

PREFIX void Spi_Teensy_LC::_selectCommand(void) {
    #if !defined(TEENSY_LC_FAST_PORT)
    digitalWriteFast(_dcPin, LOW);
    #else
    *_dcPortClear = _dcPinMask;
    #endif
}

PREFIX void Spi_Teensy_LC::_deselect(void) {
    #if !defined(TEENSY_LC_FAST_PORT)
    digitalWriteFast(_csPin, HIGH);
    #else
    *_csPortSet = _csPinMask;
    #endif
}

PREFIX void Spi_Teensy_LC::_write8(uint8_t c) {
    if (_useSPI1){
        SPI1.transfer(c);
    } else {
        SPI.transfer(c);
    }
}

PREFIX void Spi_Teensy_LC::_write16(uint16_t c) {
    if (_useSPI1){
        SPI1.transfer16(c);
    } else {
        SPI.transfer16(c);
    }
}

#undef PREFIX
