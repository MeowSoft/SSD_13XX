#include "Spi_Due.h"

#define PREFIX inline



		PREFIX void Spi_Due::startTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(spiSettings_);
			#endif
				csport_->PIO_CODR |=  cspinmask_;//LO
		}

		PREFIX void Spi_Due::endTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		PREFIX void Spi_Due::enableDataStream(void) {
			rsport_->PIO_SODR |=  dcpinmask_;//HI
		}

		PREFIX void Spi_Due::enableCommandStream(void) {
			rsport_->PIO_CODR |=  dcpinmask_;//LO
		}

		PREFIX void Spi_Due::disableCS(void) {
			csport_->PIO_SODR |=  cspinmask_;//HI
		}

		PREFIX void Spi_Due::spiwrite(uint8_t c) {
			SPI.transfer(c);
		}

		PREFIX void Spi_Due::spiwrite16(uint16_t c) {
			//SPI.transfer(c >> 8); SPI.transfer(c);
			SPI.transfer16(c);
		}

PREFIX void Spi_Due::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) { SPI.transfer16(data); }
	}

#undef PREFIX
