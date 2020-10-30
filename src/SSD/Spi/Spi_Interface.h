#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#define SPI_NAMESPACE SpiNamespace

#ifdef SPI_NAMESPACE
namespace SPI_NAMESPACE {
#endif

class Spi_Interface {

    public:

        virtual void startTransaction(void) = 0;
        virtual void endTransaction(void) = 0;
        virtual void writecommand_cont(const uint8_t c) = 0;
        virtual void writecommand16_cont(uint16_t c) = 0;
        virtual void writedata8_cont(uint8_t c) = 0;
        virtual void writedata16_cont(uint16_t d) = 0;
        virtual void writecommand_last(const uint8_t c) = 0;
        virtual void writedata8_last(uint8_t c) = 0;
        virtual void writedata16_last(uint16_t d) = 0;
};

#ifdef SPI_NAMESPACE
}
#endif

#endif
