#ifndef ADAFRUIT_984_96x64_h
#define ADAFRUIT_984_96x64_h

#include "../Registers/SSD_1331_Registers.h"
#include "SSD_DisplayData.h"

namespace SSD_Hardware {

static const uint8_t grayTable[] = {
    0x01, 0x03, 0x05, 0x07, 0x0A, 0x0D, 0x10, 0x13,
    0x16, 0x19, 0x1C, 0x20, 0x24, 0x28, 0x2C, 0x30,
    0x34, 0x38, 0x3C, 0x40, 0x44, 0x48, 0x4C, 0x50,
    0x54, 0x58, 0x5C, 0x60, 0x64, 0x68, 0x6C, 0x70
};

uint16_t SSD_DisplayData::getWidth(void)  { return 96; }
uint16_t SSD_DisplayData::getHeight(void)  { return 64; }
uint16_t SSD_DisplayData::getLineDelay(void) { return 200; }
uint16_t SSD_DisplayData::getFillDelay(void) { return 400; }
uint16_t SSD_DisplayData::getMinDelay(void) { return 4; }
bool SSD_DisplayData::hasComSplit(void) { return true; }
ColorDepth_t SSD_DisplayData::getColorDepth(void) { return ColorDepth_t::COLOR_DEPTH_16; }
ColorOrder_t SSD_DisplayData::getColorOrder(void) { return ColorOrder_t::COLOR_ORDER_RGB; }
const uint8_t* SSD_DisplayData::getCustomGrayTable(void) { return grayTable; }
uint8_t SSD_DisplayData::getContrastA(void) { return 0x91; }
uint8_t SSD_DisplayData::getContrastB(void) { return 0x50; }
uint8_t SSD_DisplayData::getContrastC(void) { return 0x7D; }
uint8_t SSD_DisplayData::getDefaultBrightness(void) { return 0x06; }
uint8_t SSD_DisplayData::getPreChargeSpeedA(void) { return 0x64; }
uint8_t SSD_DisplayData::getPreChargeSpeedB(void) { return 0x78; }
uint8_t SSD_DisplayData::getPreChargeSpeedC(void) { return 0x64; }
uint8_t SSD_DisplayData::getPreChargeLevel(void) { return 0x3A; }
uint8_t SSD_DisplayData::getStartLine(void) { return 0; }
uint16_t SSD_DisplayData::getDisplayOffset(void) { return 0; }
uint8_t SSD_DisplayData::getMuxRatio(void) { return 0x3F; }
uint8_t SSD_DisplayData::getMasterConfig(void) { return 0x8E; }
uint8_t SSD_DisplayData::getDimLevelA(void) { return 128; }
uint8_t SSD_DisplayData::getDimLevelB(void) { return 128; }
uint8_t SSD_DisplayData::getDimLevelC(void) { return 128; }
uint8_t SSD_DisplayData::getDimModePreCharge(void) { return 0x0F; }
uint8_t SSD_DisplayData::getPhasePeriod(void) { return  0b10110001; }
uint8_t SSD_DisplayData::getClockDiv(void) { return 0xF0; }
uint8_t SSD_DisplayData::getVComH(void) { return 0x3E; }

}

#endif
