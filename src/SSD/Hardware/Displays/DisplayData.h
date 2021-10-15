#ifndef DISPLAYDATA_H
#define DISPLAYDATA_H

#include <stdint.h>

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
 * @brief A struct to hold values for a 
 * particular piece of display hardware.
 */
typedef struct DisplayData {

    // // Display size.
    // uint16_t width;
    // uint16_t height;

    // Draw command delay values.
    uint16_t lineDelay;
    uint16_t fillDelay;
    uint16_t minDelay;

    // Odd / even com split.
    bool comSplit;

    // Display color depth and order.
    ColorDepth_t colorDepth;
    ColorOrder_t colorOrder;

    // Custom grayscale table values.
    const uint8_t* grayTable;

    // Pixel color contrast values.
    // CMD_CONTRASTx
    uint8_t contrastA;
    uint8_t contrastB;
    uint8_t contrastC;

    // Default brightness value. 
    // CMD_MASTERCURRENT
    uint8_t defaultBrightness;

    // Color pre charge speed settings:
    // CMD_PRECHARGEPERIOD
    // CMD_PRECHARGESPEEDx
    union {
        uint8_t preChargePeriod; // 1351 only
        uint8_t preChargeSpeedA; // 1331 only
    };
    uint8_t preChargeSpeedB; // 1331 only
    uint8_t preChargeSpeedC; // 1331 only

    // Pre charge voltage levels.
    // CMD_PRECHARGELEVELx
    union {
        uint8_t preChargeLevel; // 1331 and 1351
        uint8_t preChargeLevelA; // 1332 only
    };
    uint8_t preChargeLevelB; // 1332 only
    uint8_t preChargeLevelC; // 1332 only

    // Display row offset and start line.
    // CMD_STARTLINE
    // CMD_DISPLAYOFFSET
    uint8_t startLine;
    uint16_t displayOffset;

    // Mux ratio.
    // CMD_SETMULTIPLEX
    uint8_t muxRatio;

    // Master config settings.
    // CMD_SETCONFIG
    uint8_t masterConfig;

    // Dim mode values. (1331 only)
    // CMD_SETDIMLEVELS
    uint8_t dimModeA;
    uint8_t dimModeB;
    uint8_t dimModeC;
    uint8_t dimModePreCharge;

    // Phase period setting.
    // CMD_PHASEPERIOD
    uint8_t phasePeriod;

    // Clock div and frequency setting.
    // CMD_CLOCKDIV
    uint8_t clockDiv;

    // Com deselect level setting.
    // CMD_VCOMH
    uint8_t vcomH;

    // VSL values. (1351 only)
    // CMD_SETVSL
    uint8_t vslA;
    uint8_t vslB;
    uint8_t vslC;

} DisplayData_t;

#endif
