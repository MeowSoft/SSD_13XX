#ifndef SPI_TEENSY_3X_H
#define SPI_TEENSY_3X_H

#include "../../Spi_Interface.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

NAMESPACE_SPI

/**
 * @brief 
 * Teensy 3.x implementation of the Spi_Interface.
 */
class Spi_Teensy_3x : public Spi_Interface {

    public:

        void InitSpi(
            const uint8_t sdo,
            const uint8_t sck,
            const uint8_t cs,
            const uint8_t cd,
            uint8_t nop,
            bool initSpi,
            uint8_t* errorResult
        );

        void startTransaction(void);
        void endTransaction(void);
        void writecommand_cont(const uint8_t c);
        void writecommand16_cont(uint16_t c);
        void writedata8_cont(uint8_t c);
        void writedata16_cont(uint16_t d);
        void writecommand_last(const uint8_t c);
        void writedata8_last(uint8_t c);
        void writedata16_last(uint16_t d);
        void _pushColors_cont(uint16_t data,uint32_t times);
        void closeTransaction(void);

        #if !defined (SPI_HAS_TRANSACTION)
        void SetBitrate(uint32_t rate) override;
        #endif

    private:

        SPISettings spiSettings_;

        uint8_t csDataMask_;
        uint8_t csCommandMask_;
        uint8_t nopCmd_;

        void waitFifoNotFull(void);
        void waitTransmitComplete(uint32_t mcr);
};

#include "Spi_Teensy_3x.ipp"

NAMESPACE_SPI_END

#endif
#endif
