#include "Spi_AVR.h"

#define PREFIX inline

PREFIX void Spi_AVR::startTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.beginTransaction(_spiSettings);
    #endif

    *_csPort &= ~_csPinMask;
}

PREFIX void Spi_AVR::endTransaction(void)  {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.endTransaction();
    #endif
}

PREFIX void Spi_AVR::writeData16Multi(
    uint16_t data,
    uint32_t times
) {
    uint8_t i;
    _selectData();
    while (times--) {
        for (i = 0; i < 2; i++){
            while (!(SPSR & (1 << SPIF)));
            SPDR = (data >> (8 - (i * 8)));
        }
    }
    while (!(SPSR & (1 << SPIF)));
}

PREFIX void Spi_AVR::_selectData(void) {
    *_dcPort |= _dcPinMask;
}

PREFIX void Spi_AVR::_selectCommand(void) {
    *_dcPort &= ~_dcPinMask;
}

PREFIX void Spi_AVR::_deselect(void) {
    *_csPort |= _csPinMask;
}

PREFIX void Spi_AVR::_write8(uint8_t c) {
    SPDR = c;
    while (!(SPSR & _BV(SPIF)));
}

PREFIX void Spi_AVR::_write16(uint16_t c) {
    _spiwrite(c >> 8); 
    _spiwrite(c);
}

#undef PREFIX
