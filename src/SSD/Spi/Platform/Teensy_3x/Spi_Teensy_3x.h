#ifndef SPI_TEENSY_3X_H
#define SPI_TEENSY_3X_H

#include "../Spi_Interface.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

NAMESPACE_SPI

/**
 * @brief 
 * Teensy 3.x implementation of the Spi_Interface.
 */
class Spi_Teensy_3x : public Spi_Interface {

    public:

        bool validatePins(
            const uint8_t sdoPin, 
            const uint8_t sckPin, 
            const uint8_t csPin,
            const uint8_t dcPin,
            const char* errorMessage
        );

        void init(
            const uint8_t sdoPin, 
            const uint8_t sckPin, 
            const uint8_t csPin,
            const uint8_t dcPin,
            uint8_t nopCmd
        );

        void startTransaction(void);
        void endTransaction(void);
        void writeCommand8(const uint8_t c);
        void writeCommand16(uint16_t c);
        void writeData8(uint8_t c);
        void writeData16(uint16_t d);
        void writeCommand8AndDeselect(const uint8_t c);
        void writeData8AndDeselect(uint8_t c);
        void writeData16AndDeselect(uint16_t d);
        void writeData16Multi(uint16_t data, uint32_t times);
        void deselectAndEndTransaction(void);

        #if !defined (SPI_HAS_TRANSACTION)
        void setBitrate(uint32_t rate) override;
        #endif

    private:

        SPISettings _spiSettings;

        uint8_t _csDataMask;
        uint8_t _csCommandMask;
        uint8_t _nopCmd;

        void _waitFifoNotFull(void);
        void _waitTransmitComplete(uint32_t mcr);
};

// Inline method definitions.
#include "Spi_Teensy_3x.ipp"

NAMESPACE_SPI_END

#endif
#endif
