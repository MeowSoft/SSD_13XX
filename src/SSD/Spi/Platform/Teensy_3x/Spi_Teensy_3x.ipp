#include "Spi_Teensy_3x.h"

#define PREFIX inline

PREFIX void Spi_Teensy_3x::startTransaction(void) {
    SPI.beginTransaction(_spiSettings);
}

PREFIX void Spi_Teensy_3x::endTransaction(void) {
    SPI.endTransaction();
}

PREFIX void Spi_Teensy_3x::writeCommand8(const uint8_t c) {
    KINETISK_SPI0.PUSHR = c | (_csCommandMask << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
    _waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writeCommand16(uint16_t c) {
    KINETISK_SPI0.PUSHR = c | (_csCommandMask << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
    _waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writeData8(uint8_t d) {
    KINETISK_SPI0.PUSHR = d | (_csDataMask << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
    _waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writeData16(uint16_t d) {
    KINETISK_SPI0.PUSHR = d | (_csDataMask << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
    _waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writeCommand8AndDeselect(const uint8_t c) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = c | (_csCommandMask << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
    _waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::writeData8AndDeselect(uint8_t c) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = c | (_csDataMask << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
    _waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::writeData16AndDeselect(uint16_t d) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = d | (_csDataMask << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
    _waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::writeData16Multi(uint16_t data,uint32_t times) {
    do {
        writeData16(data);
    } while (--times > 0);
}

PREFIX void Spi_Teensy_3x::deselectAndEndTransaction(void) {
    writeCommand8AndDeselect(_nopCmd);
    endTransaction();
}

PREFIX void Spi_Teensy_3x::_waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI0.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;
    } while ((sr & (15 << 12)) > (3 << 12));
}

PREFIX void Spi_Teensy_3x::_waitTransmitComplete(uint32_t mcr) {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = KINETISK_SPI0.SR;
        if (sr & SPI_SR_EOQF) break;
        if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
    }
    KINETISK_SPI0.SR = SPI_SR_EOQF;
    SPI0_MCR = mcr;
    while (KINETISK_SPI0.SR & 0xF0) {
        tmp = KINETISK_SPI0.POPR;
    }
}

#undef PREFIX
