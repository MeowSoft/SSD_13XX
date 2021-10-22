#ifndef SSD_DISPLAYDATA_H
#define SSD_DISPLAYDATA_H

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
        virtual inline uint16_t getWidth(void);
        virtual inline uint16_t getHeight(void);

        // Delay values for certain draw operations.
        virtual inline uint16_t getLineDelay(void);
        virtual inline uint16_t getFillDelay(void);
        virtual inline uint16_t getMinDelay(void);

        // True if display glass is wired for com split.
        virtual inline bool hasComSplit(void);

        // Color depth and order.
        virtual inline ColorDepth_t getColorDepth(void);
        virtual inline ColorOrder_t getColorOrder(void);

        // Custom grayscale table. (If this is NULL then the linear table will be used.)
        virtual inline const uint8_t* getCustomGrayTable(void);

        // Contrast values.
        virtual inline uint8_t getContrastA(void);
        virtual inline uint8_t getContrastB(void);
        virtual inline uint8_t getContrastC(void);

        // Default brightness. (Sets master current control.)
        virtual inline uint8_t getDefaultBrightness(void);

        // Pre charge values.
        #if defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getPreChargePeriod(void);
        #endif
        #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
        virtual inline uint8_t getPreChargeSpeedA(void);
        virtual inline uint8_t getPreChargeSpeedB(void);
        virtual inline uint8_t getPreChargeSpeedC(void);
        #endif
        #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getPreChargeLevel(void);
        #endif
        #if defined(SSD_1332_REGISTERS_H)
        virtual inline uint8_t getPreChargeLevelA(void);
        virtual inline uint8_t getPreChargeLevelB(void);
        virtual inline uint8_t getPreChargeLevelC(void);
        #endif

        // Display start line and offset.
        virtual inline uint8_t getStartLine(void);
        virtual inline uint16_t getDisplayOffset(void);

        // Mux ratio and master config settings.
        virtual inline uint8_t getMuxRatio(void);
        virtual inline uint8_t getMasterConfig(void);

        // Values for dim mode.
        #if defined(SSD_1331_REGISTERS_H)
        virtual inline uint8_t getDimLevelA(void);
        virtual inline uint8_t getDimLevelB(void);
        virtual inline uint8_t getDimLevelC(void);
        virtual inline uint8_t getDimModePreCharge(void);
        #endif

        // Phase period, clock div and frequency, and VCom high values.
        virtual inline uint8_t getPhasePeriod(void);
        virtual inline uint8_t getClockDiv(void);
        virtual inline uint8_t getVComH(void);

        // VSL values.
        #if defined(SSD_1351_REGISTERS_H)
        virtual inline uint8_t getVSLA(void);
        virtual inline uint8_t getVSLB(void);
        virtual inline uint8_t getVSLC(void);
        #endif

        // Enhanced display.
        #if defined(SSD_1351_REGISTERS_H)
        virtual inline bool useEnhancedDisplay(void)
        #endif
};

}

#endif
