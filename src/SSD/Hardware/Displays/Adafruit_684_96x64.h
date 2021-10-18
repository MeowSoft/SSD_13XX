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

uint16_t SSD_DisplayData::getWidth()  { return 96; }
uint16_t SSD_DisplayData::getHeight()  { return 64; }
uint16_t SSD_DisplayData::getLineDelay() { return 200; }
uint16_t SSD_DisplayData::getFillDelay() { return 400; }
uint16_t SSD_DisplayData::getMinDelay() { return 4; }
bool SSD_DisplayData::hasComSplit() { return true; }
ColorDepth_t SSD_DisplayData::getColorDepth() { return ColorDepth_t::COLOR_DEPTH_16; }
ColorOrder_t SSD_DisplayData::getColorOrder() { return ColorOrder_t::COLOR_ORDER_RGB; }
const uint8_t* SSD_DisplayData::getCustomGrayTable() { return grayTable; }
uint8_t SSD_DisplayData::getContrastA() { return 0x91; }
uint8_t SSD_DisplayData::getContrastB() { return 0x50; }
uint8_t SSD_DisplayData::getContrastC() { return 0x7D; }
uint8_t SSD_DisplayData::getDefaultBrightness() { return 0x06; }
uint8_t SSD_DisplayData::getPreChargeSpeedA() { return 0x64; }
uint8_t SSD_DisplayData::getPreChargeSpeedB() { return 0x78; }
uint8_t SSD_DisplayData::getPreChargeSpeedC() { return 0x64; }
uint8_t SSD_DisplayData::getPreChargeLevel() { return 0x3A; }
uint8_t SSD_DisplayData::getStartLine() { return 0; }
uint16_t SSD_DisplayData::getDisplayOffset() { return 0; }
uint8_t SSD_DisplayData::getMuxRatio() { return 0x3F; }
uint8_t SSD_DisplayData::getMasterConfig() { return 0x8E; }
uint8_t SSD_DisplayData::getDimLevelA() { return 128; }
uint8_t SSD_DisplayData::getDimLevelB() { return 128; }
uint8_t SSD_DisplayData::getDimLevelC() { return 128; }
uint8_t SSD_DisplayData::getDimModePreCharge() { return 0x0F; }
uint8_t SSD_DisplayData::getPhasePeriod() { return  0b10110001; }
uint8_t SSD_DisplayData::getClockDiv() { return 0xF0; }
uint8_t SSD_DisplayData::getVComH() { return 0x3E; }

}

#endif
