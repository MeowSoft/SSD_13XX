#ifndef SPI_ARM_H
#define SPI_ARM_H

#include "../Base/Spi_Base.h"

// Only compile for ARM (Due)
#ifdef __SAM3X8E__

NAMESPACE_SPI

/**
 * @brief 
 * ARM implementation of the Spi_Interface.
 */
class Spi_ARM : public Spi_Base {

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

        Pio* _csPort;
        Pio* _dcPort;

        uint32_t _csPinMask;
        uint32_t _dcPinMask;

        SPISettings _spiSettings;

		void _selectData(void);
		void _selectCommand(void);
		void _deselect(void);
		void _write8(uint8_t c);
		void _write16(uint16_t c);
};

// Inline method definitions.
#include "Spi_ARM.ipp"

NAMESPACE_SPI_END

#endif
#endif
