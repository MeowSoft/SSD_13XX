#ifndef SPI_AVR_H
#define SPI_AVR_H

#include "../Base/Spi_Base.h"

// Only compile for AVR
#if defined(__AVR__)

NAMESPACE_SPI

class Spi_AVR : public Spi_Base {

    public:

        void InitSpi(
            const uint8_t cs,
            const uint8_t cd,
            bool initSpi
        );

		void startTransaction(void);
		void endTransaction(void);
        void _pushColors_cont(uint16_t data,uint32_t times);

        #if !defined (SPI_HAS_TRANSACTION)
        void SetBitrate(uint32_t rate) override;
        #endif

    private:

        volatile uint8_t 	 *csport_;
        volatile uint8_t 	 *rsport_;

        uint8_t cspinmask_;
        uint8_t dcpinmask_;

        SPISettings spiSettings_;

		void enableDataStream(void);
		void enableCommandStream(void);
		void disableCS(void);
		void spiwrite(uint8_t c);
		void spiwrite16(uint16_t c);
};

// Inline method definitions.
#include "Spi_AVR.ipp"

NAMESPACE_SPI_END

#endif
#endif
