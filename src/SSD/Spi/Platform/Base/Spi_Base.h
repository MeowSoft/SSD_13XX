#ifndef SPI_BASE_H
#define SPI_BASE_H

#include "../Spi_Interface.h"

NAMESPACE_SPI

/**
 * @brief 
 * Base class for all SPI implementations except for Teensy 3.x.
 */
class Spi_Base : public Spi_Interface {

    public:

        virtual void startTransaction(void) = 0;
        virtual void endTransaction(void) = 0;
        virtual void writeData16Multi(uint16_t data, uint32_t times) = 0;

        #if !defined (SPI_HAS_TRANSACTION)
        virtual void setBitrate(uint32_t rate);
        #endif

        void writeCommand8(const uint8_t c);
        void writeCommand16(uint16_t c);
        void writeData8(uint8_t c);
        void writeData16(uint16_t d);
        void writeCommand8AndDeselect(const uint8_t c);
        void writeData8AndDeselect(uint8_t c);
        void writeData16AndDeselect(uint16_t d);
        void deselectAndEndTransaction(void);

    protected:

        virtual void _selectData() = 0;
        virtual void _selectCommand() = 0;
        virtual void _deselect() = 0;
        virtual void _write8(uint8_t c) = 0;
        virtual void _write16(uint16_t d) = 0;
};

// Inline method definitions.
#include "Spi_Base.ipp"

NAMESPACE_SPI_END

#endif
