#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <stdint.h>

namespace SSD_Hardware {

/**
 * @brief Color depth options.
 */
typedef enum {
    COLOR_DEPTH_8 = 8,
    COLOR_DEPTH_16 = 16,
    COLOR_DEPTH_18 = 18
} ColorDepth_t;

/**
 * @brief Color order options.
 */
typedef enum {
    COLOR_ORDER_RGB,
    COLOR_ORDER_BGR
} ColorOrder_t;

/**
 * @brief This class defines data about the physical display hardware.
 */
class SSD_DisplayData {

    public:
    
        // Display width and height in pixels.
        virtual inline uint16_t getWidth();
        virtual inline uint16_t getHeight();

        // Delay values for certain draw operations.
        virtual inline uint16_t getLineDelay();
        virtual inline uint16_t getFillDelay();
        virtual inline uint16_t getMinDelay();

        // True if display glass is wired for com split.
        virtual inline bool hasComSplit();

        // Color depth and order.
        virtual inline ColorDepth_t getColorDepth();
        virtual inline ColorOrder_t getColorOrder();

        // Custom grayscale table. (If this is NULL then the linear table will be used.)
        virtual inline const uint8_t* getCustomGrayTable();

        // Contrast values.
        virtual inline uint8_t getContrastA();
        virtual inline uint8_t getContrastB();
        virtual inline uint8_t getContrastC();

        // Default brightness. (Sets master current control.)
        virtual inline uint8_t getDefaultBrightness();

        // Pre charge values.
        #if defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getPreChargePeriod();
        #endif
        #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
        virtual inline uint8_t getPreChargeSpeedA();
        virtual inline uint8_t getPreChargeSpeedB();
        virtual inline uint8_t getPreChargeSpeedC();
        #endif
        #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getPreChargeLevel();
        #endif
        #if defined(SSD_1332_REGISTERS_H)
        virtual inline uint8_t getPreChargeLevelA();
        virtual inline uint8_t getPreChargeLevelB();
        virtual inline uint8_t getPreChargeLevelC();
        #endif

        // Display start line and offset.
        virtual inline uint8_t getStartLine();
        virtual inline uint16_t getDisplayOffset();

        // Mux ratio and master config settings.
        virtual inline uint8_t getMuxRatio();
        virtual inline uint8_t getMasterConfig();

        // Values for dim mode.
        #if defined(SSD_1331_REGISTERS_H)
        virtual inline uint8_t getDimLevelA();
        virtual inline uint8_t getDimLevelB();
        virtual inline uint8_t getDimLevelC();
        virtual inline uint8_t getDimModePreCharge();
        #endif

        // Phase period, clock div and frequency, and VCom high values.
        virtual inline uint8_t getPhasePeriod();
        virtual inline uint8_t getClockDiv();
        virtual inline uint8_t getVComH();

        // VSL values.
        #if defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getVSLA();
        virtual inline uint8_t getVSLB();
        virtual inline uint8_t getVSLC();
        #endif
};

}

#endif
