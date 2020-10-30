#include "Spi_Teensy_3x.h"

#define PREFIX inline

PREFIX void Spi_Teensy_3x::startTransaction(void) {
    SPI.beginTransaction(spiSettings_);
}

PREFIX void Spi_Teensy_3x::endTransaction(void) {
    SPI.endTransaction();
}

PREFIX void Spi_Teensy_3x::writecommand_cont(const uint8_t c) {
    KINETISK_SPI0.PUSHR = c | (csCommandMask_ << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
    waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writecommand16_cont(uint16_t c) {
    KINETISK_SPI0.PUSHR = c | (csCommandMask_ << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
    waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writedata8_cont(uint8_t d) {
    KINETISK_SPI0.PUSHR = d | (csDataMask_ << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_CONT;
    waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writedata16_cont(uint16_t d) {
    KINETISK_SPI0.PUSHR = d | (csDataMask_ << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_CONT;
    waitFifoNotFull();
}

PREFIX void Spi_Teensy_3x::writecommand_last(const uint8_t c) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = c | (csCommandMask_ << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
    waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::writedata8_last(uint8_t c) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = c | (csDataMask_ << 16) | SPI_PUSHR_CTAS(0) | SPI_PUSHR_EOQ;
    waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::writedata16_last(uint16_t d) {
    uint32_t mcr = SPI0_MCR;
    KINETISK_SPI0.PUSHR = d | (csDataMask_ << 16) | SPI_PUSHR_CTAS(1) | SPI_PUSHR_EOQ;
    waitTransmitComplete(mcr);
}

PREFIX void Spi_Teensy_3x::_pushColors_cont(uint16_t data,uint32_t times) {
    do {
        writedata16_cont(data);
    } while (--times > 0);
}

PREFIX void Spi_Teensy_3x::closeTransaction(void) {
    writecommand_last(nopCmd_);
    endTransaction();
}

//Here's Paul Stoffregen SPI FIFO magic in action...
PREFIX void Spi_Teensy_3x::waitFifoNotFull(void) {
    uint32_t sr;
    uint32_t tmp __attribute__((unused));
    do {
        sr = KINETISK_SPI0.SR;
        if (sr & 0xF0) tmp = KINETISK_SPI0.POPR;  // drain RX FIFO
    } while ((sr & (15 << 12)) > (3 << 12));
}

PREFIX void Spi_Teensy_3x::waitTransmitComplete(uint32_t mcr) {
    uint32_t tmp __attribute__((unused));
    while (1) {
        uint32_t sr = KINETISK_SPI0.SR;
        if (sr & SPI_SR_EOQF) break;  // wait for last transmit
        if (sr &  0xF0) tmp = KINETISK_SPI0.POPR;
    }
    KINETISK_SPI0.SR = SPI_SR_EOQF;
    SPI0_MCR = mcr;
    while (KINETISK_SPI0.SR & 0xF0) {tmp = KINETISK_SPI0.POPR;}
}

#undef PREFIX
