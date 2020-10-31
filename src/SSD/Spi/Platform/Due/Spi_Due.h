#ifndef SPI_DUE_H
#define SPI_DUE_H

#include "../Base/Spi_Base.h"

// Only compile for ARM (Due)
#ifdef __SAM3X8E__

NAMESPACE_SPI

class Spi_Due : public Spi_Base {

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

        Pio* csport_;
        Pio* rsport_;

        uint32_t cspinmask_;
        uint32_t dcpinmask_;

        SPISettings spiSettings_;

		void enableDataStream(void);
		void enableCommandStream(void);
		void disableCS(void);
		void spiwrite(uint8_t c);
		void spiwrite16(uint16_t c);
};

// Inline method definitions.
#include "Spi_Due.ipp"

NAMESPACE_SPI_END

#endif
#endif
