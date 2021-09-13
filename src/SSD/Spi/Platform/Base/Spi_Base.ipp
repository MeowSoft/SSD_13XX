#include "Spi_Base.h"

#define PREFIX inline

PREFIX void Spi_Base::writeCommand8(const uint8_t c) { 
    _selectCommand(); 
    _write8(c); 
}

PREFIX void Spi_Base::writeCommand16(uint16_t c) { 
    _selectCommand(); 
    _write16(c); 
}

PREFIX void Spi_Base::writeData8(uint8_t c) { 
    _selectData(); 
    _write8(c); 
}

PREFIX void Spi_Base::writeData16(uint16_t d) { 
    _selectData(); 
    _write16(d); 
}

PREFIX void Spi_Base::writeCommand8AndDeselect(const uint8_t c) { 
    _selectCommand(); 
    _write8(c); 
    _deselect(); 
}

PREFIX void Spi_Base::writeData8AndDeselect(uint8_t c) { 
    _selectData(); 
    _write8(c);
    _deselect();
}

PREFIX void Spi_Base::writeData16AndDeselect(uint16_t d) { 
    _selectData(); 
    _write16(d); 
    _deselect(); 
}

PREFIX void Spi_Base::deselectAndEndTransaction(void) {
    _deselect();
    endTransaction();
}

#undef PREFIX
