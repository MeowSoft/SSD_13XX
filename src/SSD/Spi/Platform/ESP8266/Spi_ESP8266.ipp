#include "Spi_ESP8266.h"

#define PREFIX inline

        #if defined(ESP8266_FAST_CS)

		PREFIX	uint32_t Spi_ESP8266::_pinRegister(uint8_t pin) {
				return _BV(pin);
			}
		#endif

		PREFIX void Spi_ESP8266::spiwrite(uint8_t c) {
			#if defined(ESP8266_FAST_WRITE)
				SPI.write(c);
			#else
				SPI.transfer(c);
			#endif
		}

		PREFIX void Spi_ESP8266::spiwrite16(uint16_t c) {
			#if defined(ESP8266_FAST_WRITE)
				SPI.write16(c);
			#else
				SPI.transfer(c >> 8); SPI.transfer(c >> 0);
			#endif
		}

		PREFIX void Spi_ESP8266::enableCommandStream(void) {
			#if !defined(ESP8266_FAST_CS)
				digitalWrite(_dc,LOW);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_dc));//L
			#endif
		}

		PREFIX void Spi_ESP8266::enableDataStream(void) {
			#if !defined(ESP8266_FAST_CS)
				digitalWrite(_dc,HIGH);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_dc));//H
			#endif
		}

		PREFIX void Spi_ESP8266::startTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.beginTransaction(spiSettings_);
			#endif
			#if !defined(ESP8266_FAST_CS)
				digitalWrite(_cs,LOW);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_cs));//L
			#endif
		}

		PREFIX void Spi_ESP8266::endTransaction(void) {
			#if defined(SPI_HAS_TRANSACTION)
				SPI.endTransaction();
			#endif
		}

		PREFIX void Spi_ESP8266::disableCS(void) {
			#if !defined(ESP8266_FAST_CS)
				digitalWrite(_cs,HIGH);
			#else
				GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_cs));//H
			#endif
		}

        PREFIX void Spi_ESP8266::_pushColors_cont(uint16_t data,uint32_t times)
	{
		enableDataStream();
		while(times--) { spiwrite16(data); }
		//alternative faster (but currently not work at 80Mhz or more)
		//uint8_t pattern[2] = { (uint8_t)(data >> 8), (uint8_t)(data >> 0) };
		//SPI.writePattern(pattern, 2, times);
	}
#undef PREFIX
