#include "Spi_Legacy.h"

#define PREFIX inline

	PREFIX void Spi_Legacy::spiwrite(uint8_t c) {
			SPI.transfer(c);
		}

		PREFIX void Spi_Legacy::spiwrite16(uint16_t c) {
			SPI.transfer(c >> 8); SPI.transfer(c >> 0);
		}

		PREFIX void Spi_Legacy::enableCommandStream(void) {
			digitalWrite(_dc,LOW);
		}

		PREFIX void Spi_Legacy::enableDataStream(void) {
			digitalWrite(_dc,HIGH);
		}

		PREFIX void Spi_Legacy::startTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(spiSettings_);
			#endif
				digitalWrite(_cs,LOW);
		}

		PREFIX void Spi_Legacy::endTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		PREFIX void Spi_Legacy::disableCS(void) {
			digitalWrite(_cs,HIGH);
		}
        
PREFIX void Spi_Legacy::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
			while(times--) {
			SPI.transfer(data >> 8); SPI.transfer(data);
		}
	}
#undef PREFIX
