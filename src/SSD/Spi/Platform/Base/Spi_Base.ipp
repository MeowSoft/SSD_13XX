#include "Spi_Base.h"

#define PREFIX inline

PREFIX void Spi_Base::writecommand_cont(const uint8_t c) { 
    enableCommandStream(); 
    spiwrite(c); 
}

PREFIX void Spi_Base::writecommand16_cont(uint16_t c) { 
    enableCommandStream(); 
    spiwrite16(c); 
}

PREFIX void Spi_Base::writedata8_cont(uint8_t c) { 
    enableDataStream(); 
    spiwrite(c); 
}

PREFIX void Spi_Base::writedata16_cont(uint16_t d) { 
    enableDataStream(); 
    spiwrite16(d); 
}

PREFIX void Spi_Base::writecommand_last(const uint8_t c) { 
    enableCommandStream(); 
    spiwrite(c); 
    disableCS(); 
}

PREFIX void Spi_Base::writedata8_last(uint8_t c) { 
    enableDataStream(); 
    spiwrite(c);
    disableCS();
}

PREFIX void Spi_Base::writedata16_last(uint16_t d) { 
    enableDataStream(); 
    spiwrite16(d); 
    disableCS(); 
}

PREFIX void Spi_Base::closeTransaction(void)
{
    disableCS();
    endTransaction();
}

#undef PREFIX
