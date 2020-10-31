#include "Spi_Teensy_LC.h"

#define PREFIX inline

		PREFIX void Spi_Teensy_LC::spiwrite(uint8_t c) {
			if (_useSPI1){
				SPI1.transfer(c);
			} else {
				SPI.transfer(c);
			}
		}

		PREFIX void Spi_Teensy_LC::spiwrite16(uint16_t c) {
			if (_useSPI1){
				SPI1.transfer16(c);
			} else {
				SPI.transfer16(c);
			}
		}

		PREFIX void Spi_Teensy_LC::enableCommandStream(void) {
				#if !defined(TEENSY_LC_FAST_PORT)
					digitalWriteFast(dcPin,LOW);
				#else
					*dcportClear = dcpinmask;
				#endif
		}

		PREFIX void Spi_Teensy_LC::enableDataStream(void) {
				#if !defined(TEENSY_LC_FAST_PORT)
					digitalWriteFast(dcPin,HIGH);
				#else
					*dcportSet = dcpinmask;
				#endif
		}

		PREFIX void Spi_Teensy_LC::startTransaction(void) {
			if (_useSPI1){
				SPI1.beginTransaction(spiSettings_);
			} else {
				SPI.beginTransaction(spiSettings_);
			}
			#if !defined(TEENSY_LC_FAST_PORT)
				digitalWriteFast(csPin,LOW);
			#else
				*csportClear = cspinmask;
			#endif
		}

		PREFIX void Spi_Teensy_LC::disableCS(void) {
			#if !defined(TEENSY_LC_FAST_PORT)
				digitalWriteFast(csPin,HIGH);
			#else
				*csportSet = cspinmask;
			#endif
		}

		PREFIX void Spi_Teensy_LC::endTransaction(void) {
			if (_useSPI1){
				SPI1.endTransaction();
			} else {
				SPI.endTransaction();
			}
		}

PREFIX void Spi_Teensy_LC::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) {
			if (_useSPI1){
				SPI1.transfer16(data);
			} else {
				SPI.transfer16(data);
			}
		}
	}

#undef PREFIX
