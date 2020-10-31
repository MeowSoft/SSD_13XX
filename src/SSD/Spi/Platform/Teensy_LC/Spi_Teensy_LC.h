#ifndef SPI_TEENSY_LC_H
#define SPI_TEENSY_LC_H

#include "../Base/Spi_Base.h"

// Only compile for Teensy LC
#if defined(__MKL26Z64__)

// Use direct port manipulation. Comment 
// out to use digitalWriteFast() instead.
//#define TEENSY_LC_FAST_PORT

NAMESPACE_SPI

class Spi_Teensy_LC : public Spi_Base {

    public:

        void InitSpi(
            const uint8_t sdo,
            const uint8_t sck,
            const uint8_t cs,
            const uint8_t dc,
            uint8_t nop,
            bool initSpi,
            uint8_t* errorResult
        );

		void startTransaction(void);
		void endTransaction(void);
        void _pushColors_cont(uint16_t data,uint32_t times);

        #if !defined (SPI_HAS_TRANSACTION)
        void SetBitrate(uint32_t rate) override;
        #endif

    private:

		bool _useSPI1;
        SPISettings spiSettings_;

        #if defined(TEENSY_LC_FAST_PORT)
		volatile uint8_t* dcportSet;
        volatile uint8_t* dcportClear;
        volatile uint8_t* csportSet;
        volatile uint8_t* csportClear;
		uint8_t cspinmask;
        uint8_t dcpinmask;
        #else
		uint8_t csPin;
        uint8_t dcPin;
        #endif

		void enableDataStream(void);
		void enableCommandStream(void);
		void disableCS(void);
		void spiwrite(uint8_t c);
		void spiwrite16(uint16_t c);
};

// Inline method definitions.
#include "Spi_Teensy_LC.ipp"

NAMESPACE_SPI_END

#endif
#endif
