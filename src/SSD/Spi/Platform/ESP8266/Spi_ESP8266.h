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

/**
 * @brief 
 * ESP8266 implementation of the Spi_Interface.
 */
class Spi_ESP8266 : public Spi_Base {

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

		uint8_t _csPin;
        uint8_t _dcPin;
	
        #if defined(ESP8266_FAST_CS)
		uint32_t _pinRegister(uint8_t pin);
		#endif

        SPISettings _spiSettings;

		void _selectData(void);
		void _selectCommand(void);
		void _deselect(void);
		void _write8(uint8_t c);
		void _write16(uint16_t c);
};

// Inline method definitions.
#include "Spi_ESP8266.ipp"

NAMESPACE_SPI_END

#endif
#endif
