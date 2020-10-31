#ifndef SPI_ESP8266_H
#define SPI_ESP8266_H

#include "../Base/Spi_Base.h"

// Only compile for ESP8266
#if defined(ESP8266)

// Use direct port manipulation. Comment 
// out to use digitalWriteFast() instead.
#define ESP8266_FAST_CS
#define ESP8266_FAST_WRITE

NAMESPACE_SPI

class Spi_ESP8266 : public Spi_Base {

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
	
        #if defined(ESP8266_FAST_CS)
		uint32_t _pinRegister(uint8_t pin);
		#endif

        SPISettings spiSettings_;

		void enableDataStream(void);
		void enableCommandStream(void);
		void disableCS(void);
		void spiwrite(uint8_t c);
		void spiwrite16(uint16_t c);
};

// Inline method definitions.
#include "Spi_ESP8266.ipp"

NAMESPACE_SPI_END

#endif
#endif
