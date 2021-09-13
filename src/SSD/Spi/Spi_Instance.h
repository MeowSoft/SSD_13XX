#ifndef SPI_INSTANCE_H
#define SPI_INSTANCE_H

#include "Instance/Platform_Include.h"

USE_NAMESPACE_SPI

class SPI_Instance : public Spi_Interface {

    public:

        #include "Instance/ValidatePins_Declaration.h"
        #include "Instance/Init_Declaration.h"

        inline void startTransaction(void) { Spi_.startTransaction(); }
        inline void endTransaction(void) { Spi_.endTransaction(); }
        inline void writeCommand8(const uint8_t c) { Spi_.writeCommand8(c); }
        inline void writeCommand16(uint16_t c) { Spi_.writeCommand16(c); }
        inline void writeData8(uint8_t c) { Spi_.writeData8(c); }
        inline void writeData16(uint16_t d) { Spi_.writeData16(d); }
        inline void writeCommand8AndDeselect(const uint8_t c) { Spi_.writeCommand8AndDeselect(c); }
        inline void writeData8AndDeselect(uint8_t c) { Spi_.writeData8AndDeselect(c); }
        inline void writeData16AndDeselect(uint16_t d) { Spi_.writeData16AndDeselect(d); }
        inline void writeData16Multi(uint16_t data,uint32_t times) { Spi_.writeData16Multi(data, times); }
        inline void deselectAndEndTransaction(void) { Spi_.deselectAndEndTransaction(); }

    private:

        #include "Instance/Instance_Declaration.h"
};


#endif
