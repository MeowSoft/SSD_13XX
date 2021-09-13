#ifndef SPI_TEENSY_LC_H
#define SPI_TEENSY_LC_H

#include "../Base/Spi_Base.h"

// Only compile for Teensy LC
#if defined(__MKL26Z64__)

// Use direct port manipulation. Comment 
// out to use digitalWriteFast() instead.
//#define TEENSY_LC_FAST_PORT

NAMESPACE_SPI

/**
 * @brief 
 * Teensy LC implementation of the Spi_Interface.
 */
class Spi_Teensy_LC : public Spi_Base {

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
            const uint8_t dcPin
        );

		void startTransaction(void);
		void endTransaction(void);
        void writeData16Multi(uint16_t data,uint32_t times);

        #if !defined (SPI_HAS_TRANSACTION)
        void setBitrate(uint32_t rate) override;
        #endif

    private:

		bool _useSPI1;
        SPISettings _spiSettings;

        #if defined(TEENSY_LC_FAST_PORT)
		volatile uint8_t* _dcPortSet;
        volatile uint8_t* _dcPortClear;
        volatile uint8_t* _csPortSet;
        volatile uint8_t* _csPortClear;
		uint8_t _csPinMask;
        uint8_t _dcPinMask;
        #else
		uint8_t _csPin;
        uint8_t _dcPin;
        #endif

		void _selectData(void);
		void _selectCommand(void);
		void _deselect(void);
		void _write8(uint8_t c);
		void _write16(uint16_t c);
};

// Inline method definitions.
#include "Spi_Teensy_LC.ipp"

NAMESPACE_SPI_END

#endif
#endif
