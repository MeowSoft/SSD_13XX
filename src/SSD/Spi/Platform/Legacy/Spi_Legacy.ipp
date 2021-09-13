#include "Spi_Legacy.h"

#define PREFIX inline

PREFIX void Spi_Legacy::startTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.beginTransaction(spiSettings_);
    #endif

    digitalWrite(_csPin, LOW);
}

PREFIX void Spi_Legacy::endTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.endTransaction();
    #endif
}

PREFIX void Spi_Legacy::writeData16Multi(
    uint16_t data, 
    uint32_t times
) {
    _selectData();
    while (times--) {
        SPI.transfer(data >> 8); 
        SPI.transfer(data);
    }
}

PREFIX void Spi_Legacy::_selectData(void) {
    digitalWrite(_dcPin, HIGH);
}

PREFIX void Spi_Legacy::_selectCommand(void) {
    digitalWrite(_dcPin, LOW);
}

PREFIX void Spi_Legacy::_deselect(void) {
    digitalWrite(_csPin, HIGH);
}
        
PREFIX void Spi_Legacy::_write8(uint8_t c) {
    SPI.transfer(c);
}

PREFIX void Spi_Legacy::_write16(uint16_t c) {
    SPI.transfer(c >> 8); 
    SPI.transfer(c >> 0);
}

#undef PREFIX
