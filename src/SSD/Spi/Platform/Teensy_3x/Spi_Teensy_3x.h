#ifndef SPI_TEENSY_3X_H
#define SPI_TEENSY_3X_H

#include <SPI.h>

#include "../../Spi_Interface.h"

// Only compile for Teensy 3.x
#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)

#ifdef SPI_NAMESPACE
namespace SPI_NAMESPACE {
#endif

    /**
     * Struct to hold setup data for a Teensy 3x SPI instance.
     */
    struct SpiSetup {

        // Sdo and Sck pin numbers.
        const uint8_t sdo;
        const uint8_t sck;

        // CS and CD pins:
        const uint8_t cs;
        const uint8_t cd;

        // Data to write when waiting for write complete.
        const uint8_t nop;

        // Flag to init SPI hardware.
        bool initSpi;

        // Pointer to a var that will recieve an indication 
        // that SPI initialization was successful.
        bool* initSuccess;

        SpiSetup(
            const uint8_t sdo,
            const uint8_t sck,
            const uint8_t cs,
            const uint8_t cd,
            uint8_t nop,
            bool initSpi,
            bool* initSuccess
        ) : sdo(sdo), 
            sck(sck), 
            cs(cs),
            cd(cd),
            nop(nop), 
            initSpi(initSpi),
            initSuccess(initSuccess) { }
    };

    class Spi_Teensy_3x : public Spi_Interface {

        public:

            void InitSpi(SpiSetup initData);
            void startTransaction(void);
            void endTransaction(void);
            void writecommand_cont(const uint8_t c);
            void writecommand16_cont(uint16_t c);
            void writedata8_cont(uint8_t c);
            void writedata16_cont(uint16_t d);
            void writecommand_last(const uint8_t c);
            void writedata8_last(uint8_t c);
            void writedata16_last(uint16_t d);

            #if !defined (SPI_HAS_TRANSACTION)
            void SetBitrate(uint32_t rate) override;
            #endif

        private:

            SPISettings spiSettings_;

            uint8_t csDataMask_;
            uint8_t csCommandMask_;
            uint8_t nopCmd_;

            void waitFifoNotFull(void);
            void waitTransmitComplete(uint32_t mcr);
    };

// Inline method definitions.
#include "Spi_Teensy_3x.ipp"

#ifdef SPI_NAMESPACE
}
#endif

#endif

#endif
