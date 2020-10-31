#ifndef SPI_BASE_H
#define SPI_BASE_H

#include "../../Spi_Interface.h"

NAMESPACE_SPI

/**
 * @brief 
 * Base class for all SPI implementations except for Teensy 3.x.
 */
class Spi_Base : public Spi_Interface {

    public:

        virtual void startTransaction(void) = 0;
        virtual void endTransaction(void) = 0;
        virtual void _pushColors_cont(uint16_t data,uint32_t times) = 0;

        void writecommand_cont(const uint8_t c);
        void writecommand16_cont(uint16_t c);
        void writedata8_cont(uint8_t c);
        void writedata16_cont(uint16_t d);
        void writecommand_last(const uint8_t c);
        void writedata8_last(uint8_t c);
        void writedata16_last(uint16_t d);
        void closeTransaction(void);

        #if !defined (SPI_HAS_TRANSACTION)
        virtual void SetBitrate(uint32_t rate);
        #endif

    protected:

        virtual void enableDataStream() = 0;
        virtual void enableCommandStream() = 0;
        virtual void disableCS() = 0;
        virtual void spiwrite(uint8_t c) = 0;
        virtual void spiwrite16(uint16_t d) = 0;
};

#include "Spi_Base.ipp"

NAMESPACE_SPI_END

#endif
