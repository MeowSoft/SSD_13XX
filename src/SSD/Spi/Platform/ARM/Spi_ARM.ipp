#include "Spi_ARM.h"

#define PREFIX inline

PREFIX void Spi_ARM::startTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.beginTransaction(_spiSettings);
    #endif

    _csPort->PIO_CODR |= _csPinMask;
}

PREFIX void Spi_ARM::endTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.endTransaction();
    #endif
}

PREFIX void Spi_ARM::writeData16Multi(
    uint16_t data,
    uint32_t times
) {
    _selectData();
    while (times--) { 
        SPI.transfer16(data); 
    }
}

PREFIX void Spi_ARM::_selectData(void) {
    _dcPort->PIO_SODR |= _dcPinMask;
}

PREFIX void Spi_ARM::_selectCommand(void) {
    _dcPort->PIO_CODR |= _dcPinMask;
}

PREFIX void Spi_ARM::_deselect(void) {
    _csPort->PIO_SODR |= _csPinMask;
}

PREFIX void Spi_ARM::_write8(uint8_t c) {
    SPI.transfer(c);
}

PREFIX void Spi_ARM::_write16(uint16_t c) {
    SPI.transfer16(c);
}

#undef PREFIX
