#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#include "Spi_Config.h"
#include <SPI.h>

NAMESPACE_SPI 

/**
 * @brief 
 * Interface class to define SPI methods for SSD_13XX.
 */
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
        virtual void _pushColors_cont(uint16_t data,uint32_t times) = 0;
        virtual void closeTransaction(void) = 0;

        #if !defined (SPI_HAS_TRANSACTION)
        virtual void SetBitrate(uint32_t rate) = 0;
        #endif
};

NAMESPACE_SPI_END

#endif
