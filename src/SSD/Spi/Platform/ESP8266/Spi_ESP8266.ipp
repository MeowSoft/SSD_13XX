#include "Spi_ESP8266.h"

#define PREFIX inline

PREFIX void Spi_ESP8266::startTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.beginTransaction(_spiSettings);
    #endif

    #if !defined(ESP8266_FAST_CS)
    digitalWrite(_csPin, LOW);
    #else
    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_csPin));
    #endif
}

PREFIX void Spi_ESP8266::endTransaction(void) {
    #if defined(SPI_HAS_TRANSACTION)
    SPI.endTransaction();
    #endif
}

PREFIX void Spi_ESP8266::writeData16Multi(
    uint16_t data, 
    uint32_t times
) {
    _selectData();
    while (times--) { 
        _write16(data); 
    }
}

#if defined(ESP8266_FAST_CS)
PREFIX uint32_t Spi_ESP8266::_pinRegister(uint8_t pin) {
    return _BV(pin);
}
#endif

PREFIX void Spi_ESP8266::_selectData(void) {
    #if !defined(ESP8266_FAST_CS)
    digitalWrite(_dcPin, HIGH);
    #else
    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_dcPin));
    #endif
}

PREFIX void Spi_ESP8266::_selectCommand(void) {
    #if !defined(ESP8266_FAST_CS)
    digitalWrite(_dcPin, LOW);
    #else
    GPIO_REG_WRITE(GPIO_OUT_W1TC_ADDRESS, _pinRegister(_dcPin));
    #endif
}

PREFIX void Spi_ESP8266::_deselect(void) {
    #if !defined(ESP8266_FAST_CS)
    digitalWrite(_csPin, HIGH);
    #else
    GPIO_REG_WRITE(GPIO_OUT_W1TS_ADDRESS, _pinRegister(_csPin));
    #endif
}

PREFIX void Spi_ESP8266::_write8(uint8_t c) {
    #if defined(ESP8266_FAST_WRITE)
    SPI.write(c);
    #else
    SPI.transfer(c);
    #endif
}

PREFIX void Spi_ESP8266::_write16(uint16_t c) {
    #if defined(ESP8266_FAST_WRITE)
    SPI.write16(c);
    #else
    SPI.transfer(c >> 8); 
    SPI.transfer(c >> 0);
    #endif
}
 
#undef PREFIX
