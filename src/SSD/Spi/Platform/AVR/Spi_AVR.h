#ifndef SPI_AVR_H
#define SPI_AVR_H

#include "../Base/Spi_Base.h"

// Only compile for AVR
#if defined(__AVR__)

NAMESPACE_SPI

/**
 * @brief 
 * AVR implementation of the Spi_Interface.
 */
class Spi_AVR : public Spi_Base {

    public:

        void init(
            const uint8_t csPin,
            const uint8_t dcPin
        );

		void startTransaction(void);
		void endTransaction(void);
        void writeData16Multi(uint16_t data, uint32_t times);

        #if !defined (SPI_HAS_TRANSACTION)
        void setBitrate(uint32_t rate) override;
        #endif

    private:

        volatile uint8_t* _csPort;
        volatile uint8_t* _dcPort;

        uint8_t _csPinMask;
        uint8_t _dcPinMask;

        SPISettings _spiSettings;

		void _selectData(void);
		void _selectCommand(void);
		void _deselect(void);
		void _write8(uint8_t c);
		void _write16(uint16_t c);
};

// Inline method definitions.
#include "Spi_AVR.ipp"

NAMESPACE_SPI_END

#endif
#endif
