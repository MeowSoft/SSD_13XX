#include "Spi_AVR.h"

#define PREFIX inline

PREFIX void Spi_AVR::spiwrite(uint8_t c) {
    SPDR = c;//load your outgoing data into the SPI shift reg's data register
    while(!(SPSR & _BV(SPIF)));//wait for the data to be transmitted on MOSI
}

PREFIX void Spi_AVR::spiwrite16(uint16_t c) {
    spiwrite(c >> 8); spiwrite(c);
}

PREFIX void Spi_AVR::enableCommandStream(void) {
    *rsport_ &= ~dcpinmask_;//low
}

PREFIX void Spi_AVR::enableDataStream(void) {
    *rsport_ |= dcpinmask_;//hi
}

PREFIX void Spi_AVR::startTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
        SPI.beginTransaction(spiSettings_);
    #endif
        *csport_ &= ~cspinmask_;//low
}

PREFIX void Spi_AVR::endTransaction(void)  {
    #if defined(SPI_HAS_TRANSACTION)
        SPI.endTransaction();
    #endif
}

PREFIX void Spi_AVR::disableCS(void) {
    *csport_ |= cspinmask_;//hi
}

PREFIX void Spi_AVR::_pushColors_cont(uint16_t data,uint32_t times)
{
    uint8_t i;
    enableDataStream();
    while(times--) {
        for (i=0;i<2;i++){
            while(!(SPSR & (1 << SPIF)));
            SPDR = (data >> (8 - (i*8)));
        }
    }
    while(!(SPSR & (1 << SPIF)));
}

#undef PREFIX
