#ifndef SPIINTERFACE_H
#define SPIINTERFACE_H

#include "../Spi_Config.h"
#include <SPI.h>

NAMESPACE_SPI 

/**
 * @brief 
 * Interface class to define SPI methods for SSD_13XX.
 * 
 * @details
 * This interface is implemented by each platform SPI implementation. 
 * This ensures each platform implementation has the same behavior.
 */
class Spi_Interface {

    public:

        // Start SPI transaction
        virtual void startTransaction(void) = 0;

        // End SPI transaction
        virtual void endTransaction(void) = 0;

        // Write an 8 bit command
        virtual void writeCommand8(const uint8_t c) = 0;

        // Write a 16 bit command
        virtual void writeCommand16(uint16_t c) = 0;

        // Write 8 bits of data
        virtual void writeData8(uint8_t c) = 0;

        // Write 16 bits of data
        virtual void writeData16(uint16_t d) = 0;

        // Write 16 bits of data multiple times
        virtual void writeData16Multi(uint16_t data, uint32_t times) = 0;

        // Write an 8 bit command, wait, and deselect
        virtual void writeCommand8AndDeselect(const uint8_t c) = 0;

        // Write 8 bits of data, wait, and deselect
        virtual void writeData8AndDeselect(uint8_t c) = 0;

        // Write 16 bits of data, wait, and deselect
        virtual void writeData16AndDeselect(uint16_t d) = 0;

        // Wait for any remaining data to transmit,
        // deselect, and end the transaction
        virtual void deselectAndEndTransaction(void) = 0;

        // Set SPI bitrate (if the SPI implementation is using transactions, 
        // then bitrate is set with SPISetting(clock, bitOrder, dataMode))
        #if !defined (SPI_HAS_TRANSACTION)
        virtual void SetBitrate(uint32_t rate) = 0;
        #endif
};

NAMESPACE_SPI_END

#endif
