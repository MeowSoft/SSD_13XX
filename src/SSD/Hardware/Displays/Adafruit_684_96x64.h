#ifndef ADAFRUIT_984_96x64_h
#define ADAFRUIT_984_96x64_h

#include "DisplayData.h"
#include "../Registers/SSD_1331_Registers.h"

#define SSD_WIDTH 96
#define SSD_HEIGHT 64


/**
 * @brief Custom grayscale table to use.
 */
static const uint8_t grayTable[] = {
    0x01, 0x03, 0x05, 0x07, 0x0A, 0x0D, 0x10, 0x13,
    0x16, 0x19, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30,
    0x34, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x50,
    0x54, 0x58, 0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70
};

/**
 * @brief Definitions for Adafruit 684 96x64 OLED breakout board.
 */
static const DisplayData_t adaFruit_684_96x64_DisplayData = {

    // // Set width and height.
    // /* width: */ 96,
    // /* height: */ 64,

    // Set drawing delay values.
    /* lineDelay: */ 200,
    /* fillDelay: */ 400,
    /* minDelay: */ 4,

    // This display is set up for com split.
    /* comSplit: */ true,

    // Set color depth and order.
    /* colorDepth: */ ColorDepth_t::COLOR_DEPTH_16,
    /* colorOrder: */ ColorOrder_t::COLOR_ORDER_RGB,

    // Use custom grayscale table.
    /* grayTable: */ grayTable,

    // Set default contrast values.
    /* contrastA: */ 0x91,
    /* contrastB: */ 0x50,
    /* contrastC: */ 0x7D,

    // Set default brightness values.
    /* defaultBrightness: */ 0x06,

    // Set precharge speeds.
    { /* preChargeSpeedA: */ 0x64 },
    /* preChargeSpeedB: */ 0x78,
    /* preChargeSpeedC: */ 0x64,

    // Set precharge level.
    { /* preChargeLevel: */ 0x3A },
    0, 
    0,
    
    // Set start line and offset.
    /* startLine: */ 0,
    /* displayOffset: */ 0,

    // Set MUX ratio and config.
    /* muxRatio: */ 0x3F,
    /* masterConfig: */ 0x8E,

    // Set default dim mode values.
    /* dimModeA: */ 128,
    /* dimModeB: */ 128,
    /* dimModeC: */ 128,
    /* dimModePreCharge: */ 0x0F,

    // Set phase period, clock, and vcom settings.
    /* phasePeriod: */ 0b10110001,
    /* clockDiv: */ 0xF0,
    /* vcomH: */ 0x3E
};

#endif
