#ifndef SPI_LEGACY_H
#define SPI_LEGACY_H


#include "../Base/Spi_Base.h"

// Don't compile if the platform is implemented elsewhere.
#if defined(PLATFORM_LEGACY)

NAMESPACE_SPI

class Spi_Legacy : public Spi_Base {

    public:

        void InitSpi(
            const uint8_t cs,
            const uint8_t dc,
            bool initSpi
        );

		void startTransaction(void);
		void endTransaction(void);
        void _pushColors_cont(uint16_t data,uint32_t times);

        #if !defined (SPI_HAS_TRANSACTION)
        void SetBitrate(uint32_t rate) override;
        #endif

    private:

			uint8_t 			_cs,_dc;
	
        SPISettings spiSettings_;

		void enableDataStream(void);
		void enableCommandStream(void);
		void disableCS(void);
		void spiwrite(uint8_t c);
		void spiwrite16(uint16_t c);
};

// Inline method definitions.
#include "Spi_Legacy.ipp"

NAMESPACE_SPI_END

#endif
#endif
