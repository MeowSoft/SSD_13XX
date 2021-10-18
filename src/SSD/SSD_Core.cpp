#include "SSD_Core.h"
#include "SSD_Util.h"

/**
 * Constructor.
 */
SSD_Core::SSD_Core(
    SPI_Driver spi, 
    const uint8_t rstPin
) {
    _rstPin  = rstPin;
    _spi = spi;
}

// ===================================================
/* #region Screen configuration methods:            */
// ===================================================

void SSD_Core::init(void) {

    // Initialize screen config.
    _screenConfig.init(this);
	_screenConfig.setColorDepth(SSD_DISPLAY_DATA_COLOR_DEPTH);
	_screenConfig.setColorOrder(SSD_DISPLAY_DATA_COLOR_ORDER);

    // Reset screen hardware.
	resetScreen();
	delay(30);

    // Start SPI.
	_spi.startTransaction();

    // Initialize the SSD chip.
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	    _init133x();
	#elif defined(SSD_1351_REGISTERS_H)
	    _init1351();
	#endif

    // Set normal display mode and turn on.
	_spi.writeCommand8(CMD_NORMALDISPLAY);
	_spi.writeCommand8AndDeselect(CMD_DISPLAYON);

    // Finish SPI.
	_spi.endTransaction();
	delay(60);

    // Set screen rotation.
	setScreenRotation(SSD_ScreenConfig::ROTATION_LANDSCAPE);

    // Clear screen.
	fillScreen(0);
	delay(1);

    // Set mode in screen config.
	_screenConfig.changeMode(SSD_ScreenConfig::NORMAL);
}

void SSD_Core::resetScreen(void) {
    if (_rstPin != 255) {
		pinMode(_rstPin, OUTPUT);
		digitalWrite(_rstPin, HIGH);
		delay(10);
		digitalWrite(_rstPin, LOW);
		delay(10);
		digitalWrite(_rstPin, HIGH);
		delay(10);
	}
}

void SSD_Core::setScreenMode(const enum SSD_ScreenConfig::ScreenModes mode) {
	_screenConfig.changeMode(mode);
}

SSD_ScreenConfig::ScreenModes SSD_Core::getScreenMode(void) {
	return _screenConfig.getMode();
}

void SSD_Core::setScreenRotation(SSD_ScreenConfig::Rotations rotation) {
    _screenConfig.setRotation(rotation);
    _screenConfig.writeRemap();
}

SSD_ScreenConfig::Rotations SSD_Core::getScreenRotation(void) {
	return _screenConfig.getRotation();
}

int16_t SSD_Core::getScreenHeight(void) const {
	return _screenConfig.getHeight();
}

int16_t SSD_Core::getScreenWidth(void) const {
	return _screenConfig.getWidth();
}

void SSD_Core::setScreenColorDepth(uint8_t depth) {
	_screenConfig.setColorDepth(depth);
}

void SSD_Core::setScreenColorOrder(SSD_Hardware::ColorOrder_t order) {
    bool useBGR = (order == SSD_Hardware::ColorOrder_t::COLOR_ORDER_BGR);
	_screenConfig.setColorOrder(useBGR);
}

void SSD_Core::setAddressWindow(
    int16_t x0, 
    int16_t y0, 
    int16_t x1, 
    int16_t y1
) {
	_spi.startTransaction();
	_setAddressWindow(x0, y0, x1, y1, true);
	_spi.deselectAndEndTransaction();
}

void SSD_Core::setScreenBrightness(uint8_t brightness) {
	// Clamp to 15.
	if (brightness > 15) brightness = 15;

    // Set brightness.
	_spi.startTransaction();
    _writeRegister(CMD_MASTERCURRENT, brightness);
	_spi.endTransaction();
}

/* #endregion */
// ===================================================

// ===================================================
/* #region Scroll methods:                          */
// ===================================================

void SSD_Core::defineScrollArea(
    uint8_t a, 
    uint8_t b, 
    uint8_t c, 
    uint8_t d, 
    uint8_t e
) {
    // If scroll area is off screen then bail.
	if (b + c > SSD_DISPLAY_DATA_HEIGHT) return;

    // Scroll speed bitfield.
	uint8_t scrollSpeed = 0;

	#if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)

        // Set scroll speed field.
		if (e == 0){
			scrollSpeed = 0b00000000;
		} else if (e == 1){
			scrollSpeed = 0b00000001;
		} else if (e == 2){
			scrollSpeed = 0b00000010;
		} else {
			scrollSpeed = 0b00000011;
		}

        // Set scroll area command.
		_spi.startTransaction();
		_spi.writeCommand8(CMD_SCROLL_SETUP);
		_spi.writeCommand8(a);
		_spi.writeCommand8(b);
		_spi.writeCommand8(c);
		_spi.writeCommand8(d);
		_spi.writeCommand8AndDeselect(scrollSpeed);
		_spi.endTransaction();

	#elif defined(SSD_1351_REGISTERS_H)

        // Set scroll speed field.
		if (e == 0){
			scrollSpeed = 0b00000001;
		} else if (e == 1){
			scrollSpeed = 0b00000010;
		} else 
			scrollSpeed = 0b00000011;
		}

        // Set scroll area command.
		_spi.startTransaction();
		_spi.writeCommand8(CMD_HORIZSCROLL);
		_spi.writeData8(a);
		_spi.writeData8(b);
		_spi.writeData8(c);
		_spi.writeData8(0);
		_spi.writeData8AndDeselect(scrollSpeed);
		_spi.endTransaction();

	#endif
}

void SSD_Core::scroll(bool active) {
    uint8_t cmd = active ? CMD_SCROLL_ON : CMD_SCROLL_OFF;
	_spi.startTransaction();
	_spi.writeCommand8AndDeselect(cmd);
	_spi.endTransaction();
}

/* #endregion */
// ===================================================

// ===================================================
/* #region Area methods:                            */
// ===================================================

void SSD_Core::copyArea(
    int16_t sourceX, 
    int16_t sourceY, 
    int16_t width, 
    int16_t height,
    int16_t destinationX, 
    int16_t destinationY
) {
    // Area copy is only available for the 1331 and 1332.
    #if !defined(SSD_1331_REGISTERS_H) && \
        !defined(SSD_1332_REGISTERS_H)
        return;
    #endif

    int16_t x0, y0, x1, y1;
    x0 = sourceX;
    y0 = sourceY;
    x1 = x0 + width;
    y1 = y0 + height;

    // Handle portrait mode.
	if (_screenConfig.isPortrait()){
		swapVals(x0, y0);
		swapVals(x1, y1);
		swapVals(destinationX, destinationY);
	}

    // Copy area.
	_spi.startTransaction();
	_spi.writeCommand8(CMD_COPYWINDOW);
	_spi.writeCommand8(x0 & 0xFF);
	_spi.writeCommand8(y0 & 0xFF);
	_spi.writeCommand8(x1 & 0xFF);
	_spi.writeCommand8(y1 & 0xFF);
	_spi.writeCommand8(destinationX & 0xFF);
	_spi.writeCommand8AndDeselect(destinationY & 0xFF);
	_spi.endTransaction();
}

void SSD_Core::dimArea(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height
) {
    // Area dim is only available for the 1331 and 1332.
    #if !defined(SSD_1331_REGISTERS_H) && \
        !defined(SSD_1332_REGISTERS_H)
        return;
    #endif

    int16_t x0, y0, x1, y1;
    x0 = x;
    y0 = y;
    x1 = x0 + width;
    y1 = y0 + height;

    // Handle portrait mode.
	if (_screenConfig.isPortrait()){
		swapVals(x0, y0);
		swapVals(x1, y1);
	}

    // Dim area.
	_spi.startTransaction();
	_spi.writeCommand8(CMD_DIMWINDOW);
	_spi.writeCommand8(x0 & 0xFF);
	_spi.writeCommand8(y0 & 0xFF);
	_spi.writeCommand8(x1 & 0xFF);
	_spi.writeCommand8AndDeselect(y1 & 0xFF);
	_spi.endTransaction();
}

void SSD_Core::clearArea(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height
) {
    // Area move is only available for the 1331 and 1332.
    #if !defined(SSD_1331_REGISTERS_H) && \
        !defined(SSD_1332_REGISTERS_H)
        return;
    #endif

    int16_t x0, y0, x1, y1;
    x0 = x;
    y0 = y;
    x1 = x0 + width;
    y1 = y0 + height;

    // Handle portrait mode.
	if (_screenConfig.isPortrait()){
		swapVals(x0, y0);
		swapVals(x1, y1);
	}

    // Move area.
	_spi.startTransaction();
	_spi.writeCommand8(CMD_CLRWINDOW);
	_spi.writeCommand8(x0 & 0xFF);
	_spi.writeCommand8(y0 & 0xFF);
	_spi.writeCommand8(x1 & 0xFF);
	_spi.writeCommand8AndDeselect(y1 & 0xFF);
	_spi.endTransaction();
}

/* #endregion */
// ===================================================

// ===================================================
/* #region Drawing methods:                         */
// ===================================================

void SSD_Core::fillScreen(uint16_t color) {

    // SSD_1331 or SSD_1332:
	#if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)

        // Convert color data.
		uint8_t r1, g1, b1;
		SSD_Util::_convertColor(color, r1, g1, b1);

        // Fill screen.
		_spi.startTransaction();
		_setFillState(1);
		_spi.writeCommand8(CMD_DRAWRECT);
		_spi.writeCommand16(0);
		_spi.writeCommand8(SSD_DISPLAY_DATA_WIDTH - 1);
		_spi.writeCommand8(SSD_DISPLAY_DATA_HEIGHT - 1);
		_writeColorData(r1, g1, b1);
		_writeColorData(r1, g1, b1);
		delayMicroseconds(SSD_DISPLAY_DATA_FILL_DELAY);
	    _spi.deselectAndEndTransaction();

    // SSD_1351:
	#else

        // Fill screen.
		_spi.startTransaction();
		_setAddressWindow(0, 0, SSD_WIDTH - 1, SSD_HEIGHT - 1, false);
		_spi.writeData16Multi(color, SSD_WIDTH * SSD_HEIGHT);
	    _spi.deselectAndEndTransaction();

	#endif
}

void SSD_Core::drawPixel(
    int16_t x, 
    int16_t y, 
    uint16_t color
) {
	_spi.startTransaction();
	_drawPixel(x, y, color);
	_spi.deselectAndEndTransaction();
}

void SSD_Core::drawLine(
    int16_t x0, 
    int16_t y0,
    int16_t x1, 
    int16_t y1, 
    uint16_t color
) {
	_spi.startTransaction();

    // Horizontal line.
    if (y0 == y1) {
        _drawHorizontalLine(x0, y0, x1 - x0, color);
    }

    // Vertical line.
    else if (x0 == x1) {
        _drawVerticalLine(x0, y0, y1 - y0, color);
    }

    // Diagonal line.
    else {
	    _drawLine(x0, y0, x1, y1, color);
    }

	_spi.deselectAndEndTransaction();
}

void SSD_Core::drawRectangle(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height, 
    uint16_t color1,
    uint16_t color2,
    bool filled
) {
	_spi.startTransaction();
	_drawRectangle(x, y, width, height, color1, color2, filled);
	_spi.deselectAndEndTransaction();
}

void SSD_Core::drawGradient(
    int16_t x, 
    int16_t y, 
    int16_t width, 
    int16_t height, 
    uint16_t color1,
    uint16_t color2
) {
	_spi.startTransaction();
	_drawGradient(x, y, width, height, color1, color2);
	_spi.deselectAndEndTransaction();
}

/* #endregion */
// ===================================================

// ===================================================
/* #region Private methods:                         */
// ===================================================

void SSD_Core::_init133x() {
    _spi.writeCommand8(CMD_DISPLAYOFF);
    _writeRegister(CMD_CLOCKDIV, SSD_DISPLAY_DATA_CLOCK_DIV);

    // Set MUX ratio.
    _writeRegister(CMD_SETMULTIPLEX, SSD_DISPLAY_DATA_MUX_RATIO);
    
    // Set display start line and offset.
    _writeRegister(CMD_STARTLINE, SSD_DISPLAY_DATA_START_LINE);
    _writeRegister(CMD_DISPLAYOFFSET, SSD_DISPLAY_DATA_OFFSET);

    _writeRegister(CMD_SETCONFIG, SSD_DISPLAY_DATA_MASTER_CONFIG);
    _writeRegister(CMD_POWERMODE, VAL_POWERMODE_OFF);
    _writeRegister(CMD_MASTERCURRENT, SSD_DISPLAY_DATA_DEFAULT_BRIGHTNESS);
    _setGreyscaleTable();
    _setContrast();
    _setDimModeContrast();
    
_writeRegister(CMD_PHASEPERIOD, SSD_DISPLAY_DATA_PHASE_PERIOD);
    _setPreCharge();
    _writeRegister(CMD_VCOMH, SSD_DISPLAY_DATA_VCOMH);
    _setFillState(1);
}

void SSD_Core::_init1351() {
    #if defined(SSD_1351_REGISTERS_H)
        _writeRegister(CMD_CMDLOCK, SSD_COMMANDLOCK1);
        _writeRegister(CMD_CMDLOCK, SSD_COMMANDLOCK2);
        _spi.writeCommand8(CMD_DISPLAYOFF);
        _writeRegister(CMD_CLOCKDIV, displayData->clockDiv);


    // Set MUX ratio.
        _writeRegister(CMD_SETMULTIPLEX, displayData->muxRatio);

        // Set display start line and offset.
        _writeRegister(CMD_STARTLINE, displayData->startLine);
        _writeRegister(CMD_DISPLAYOFFSET, displayData->displayOffset);

        _writeRegister(CMD_SETGPIO, SSD_SETGPIO);
        _writeRegister(CMD_SETCONFIG, displayData->masterConfig);

        _spi.writeCommand8(CMD_SETVSL);
        _spi.writeData8(displayData->vslA);
        _spi.writeData8(displayData->vslB);
        _spi.writeData8(displayData->vslC);

        _setContrast();

        _writeRegister(CMD_MASTERCURRENT, SSD_MASTERCURRENT);

            _setEnhanceDisplay(SSD_ENHANCE)
    

        _setGreyscaleTable();
        _setPreCharge();
        _writeRegister(CMD_PHASEPERIOD, displayData->phasePeriod);
        _writeRegister(CMD_VCOMH, displayData->vcomH);
    #endif
}

/**
 *                              1331:   1332:   1351:   Method:
 * 
 * CMD_SETCOLUMN                0x15    0x15    0x15    SSD_13XX::_setAddressWindow
 * CMD_SETROW                   0x75    0x75    0x75    SSD_13XX::_setAddressWindow
 * 
 * CMD_WRITERAM                 ----    ----    0x5C
 * CMD_READRAM                  ----    ----    0x5D
 *      
 * CMD_CONTRASTA                0x81    0x81    ----    SSD_13XX::_setContrast                  
 * CMD_CONTRASTB                0x82    0x82    ----    SSD_13XX::_setContrast
 * CMD_CONTRASTC                0x83    0x83    ----    SSD_13XX::_setContrast
 * CMD_CONTRASTABC              ----    ----    0xC1    SSD_13XX::_setContrast
 *      
 * CMD_MASTERCURRENT            0x87    0x87    0xC7    SSD_13XX::setScreenBrightness
 * 
 * CMD_PRECHARGESPEEDA          0x8A    ----    ----    SSD_13XX::_setPreCharge
 * CMD_PRECHARGESPEEDB          0x8B    ----    ----    SSD_13XX::_setPreCharge
 * CMD_PRECHARGESPEEDC          0x8C    ----    ----    SSD_13XX::_setPreCharge
 * 
 * CMD_SETREMAP                 0xA0    0xA0    0xA0    SSD_ScreenConfig::writeRemap
 * 
 * CMD_STARTLINE                0xA1    0xA1    0xA1    init
 * CMD_DISPLAYOFFSET            0xA2    0xA2    0xA2    init
 * 
 * CMD_NORMALDISPLAY            0xA4    0xA4    0xA6    SSD_ScreenConfig::changeMode
 * CMD_DISPLAYALLON             0xA5    0xA5    0xA5    SSD_ScreenConfig::changeMode
 * CMD_DISPLAYALLOFF            0xA6    0xA6    0xA4    SSD_ScreenConfig::changeMode
 * CMD_INVERTDISPLAY            0xA7    0xA7    0xA7    SSD_ScreenConfig::changeMode
 * 
 * CMD_SETMULTIPLEX             0xA8    0xA8    0xCA    init
 *      
 * CMD_SETDIMLEVELS             0xAB    ----    ----    SSD_13XX::_setDimModeContrast
 *      
 * CMD_SETCONFIG                0xAD    0xAD    0xAB    init
 * 
 * CMD_DISPLAYDIM               0xAC    ----    ----    SSD_ScreenConfig::changeMode
 * CMD_DISPLAYOFF               0xAE    0xAE    0xAF    SSD_ScreenConfig::changeMode
 * CMD_DISPLAYON                0xAF    0xAF    0xAE    SSD_ScreenConfig::changeMode
 *      
 * CMD_POWERMODE                0xB0    0xB0    ----    SSD_ScreenConfig::changeMode
 *
 * CMD_DISPLAYENHANCE           ----    ----    0xB2    SSD_13XX::_setEnhanceDisplay
 *
 * CMD_PHASEPERIOD              0xB1    0xB1    0xB1    init
 *      
 * CMD_SETCLOCKDIV              0xB3    0xB3    0xB3    init
 * 
 * CMD_SETVSL                   ----    ----    0xB4    init
 * 
 * CMD_SETGPIO                  ----    ----    0xB5    init
 * 
 * CMD_PRECHARGEPERIOD          ----    ----    0xB6    SSD_13XX::_setPreCharge
 *
 * CMD_SETGRAYSCALE             0xB8    0xB8    0xB8    SSD_13XX::_setGreyscaleTable
 * CMD_LINEARGRAY               0xB9    0xB9    0xB9    SSD_13XX::_setGreyscaleTable
 * 
 * CMD_PRECHARGELEVEL           0xBB    ----    0xBB    SSD_13XX::_setPreCharge
 * CMD_PRECHARGELEVELA          ----    0xBB    ----    SSD_13XX::_setPreCharge
 * CMD_PRECHARGELEVELB          ----    0xBC    ----    SSD_13XX::_setPreCharge
 * CMD_PRECHARGELEVELC          ----    0xBD    ----    SSD_13XX::_setPreCharge
 * 
 * CMD_SETVCOMH                 0xBE    0xBE    0xBE    init
 * 
 * CMD_NOP                      0xE3    0xE3    0xE3
 * CMD_LOCK                     0xFD    ----    0xFD
 * 
 * CMD_DRAWLINE                 0x21    0x21    ----
 * CMD_DRAWRECT                 0x22    0x22    ----
 * CMD_COPYWINDOW               0x23    0x23    ----
 * CMD_DIMWINDOW                0x24    0x24    ----
 * CMD_CLRWINDOW                0x25    0x25    ----
 * CMD_FILLMODE                 0x26    0x26    ----
 * 
 * CMD_SCROLL_SETUP             0x27    ----    0x96
 * CMD_SCROLL_OFF               0x2E    ----    0x9E
 * CMD_SCROLL_ON                0x2F    ----    0x9F
 */


void SSD_Core::_setContrast() {

    // SSD_1331 or SSD_1332:
    #if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)
        _writeRegister(CMD_CONTRASTA, SSD_DISPLAY_DATA_CONTRAST_A);
        _writeRegister(CMD_CONTRASTB, SSD_DISPLAY_DATA_CONTRAST_B);
        _writeRegister(CMD_CONTRASTC, SSD_DISPLAY_DATA_CONTRAST_C);

    // SSD_1351:
    #elif defined(SSD_1351_REGISTERS_H)
        _spi.writeCommand8(CMD_CONTRASTABC);
        _spi.writeData8(displayData->contrastA);
        _spi.writeData8(displayData->contrastB);
        _spi.writeData8(displayData->contrastC);
    #endif
}

void SSD_Core::_setPreCharge() {
    #if defined(SSD_1331_REGISTERS_H)
        _writeRegister(CMD_PRECHARGESPEEDA, SSD_DISPLAY_DATA_PRECHARGE_SPEED_A);
        _writeRegister(CMD_PRECHARGESPEEDB, SSD_DISPLAY_DATA_PRECHARGE_SPEED_B);
        _writeRegister(CMD_PRECHARGESPEEDC, SSD_DISPLAY_DATA_PRECHARGE_SPEED_C);
        _writeRegister(CMD_PRECHARGELEVEL, SSD_DISPLAY_DATA_PRECHARGE_LEVEL);
    #elif defined(SSD_1332_REGISTERS_H)
		_writeRegister(CMD_PRECHARGELEVELA, displayData->preChargeLevelA);
		_writeRegister(CMD_PRECHARGELEVELB, displayData->preChargeLevelB);
		_writeRegister(CMD_PRECHARGELEVELC, displayData->preChargeLevelC);
    #elif defined(SSD_1351_REGISTERS_H)
		_writeRegister(CMD_PRECHARGEPERIOD, displayData->preChargePeriod);
        _writeRegister(CMD_PRECHARGELEVEL, displayData->preChargeLevel);
    #endif
}



void SSD_Core::_setDimModeContrast() {
     #if defined(SSD_1331_REGISTERS_H)
        _spi.writeCommand8(CMD_SETDIMLEVELS);
        _spi.writeCommand8(0);
        _spi.writeCommand8(SSD_DISPLAY_DATA_DIM_LEVEL_A);
        _spi.writeCommand8(SSD_DISPLAY_DATA_DIM_LEVEL_B);
        _spi.writeCommand8(SSD_DISPLAY_DATA_DIM_LEVEL_C);
        _spi.writeCommand8(SSD_DISPLAY_DATA_DIM_MODE_PRECHARGE);
    #endif
}



void SSD_Core::_setEnhanceDisplay(bool enhance) {
    #if defined(SSD_1351_REGISTERS_H)
    uint8_t* data = enhance 
        ? VAL_DISPLAYENHANCE_ON 
        : VAL_DISPLAYENHANCE_OFF;

    _spi.writeCommand8(CMD_DISPLAYENHANCE);
    _spi.writeData8(data[0]);
    _spi.writeData8(data[1]);
    _spi.writeData8(data[2]);
    #endif
}



void SSD_Core::_setGreyscaleTable() {

    // If no custom values are defined...
    if (SSD_DISPLAY_DATA_GRAY_TABLE == NULL) {

        // Use the linear table.
        _spi.writeCommand8(CMD_LINEARGRAY);
        return;
    }

    // Otherwise, write custom table data.
    _spi.writeCommand8(CMD_GRAYSCALE); 

    for (uint8_t i = 0; i < 32; i++){
        #if defined(SSD_CMD_DATA_BIT)
            _spi.writeData8(displayData->grayTable[i]);
        #else
            _spi.writeCommand8(SSD_DISPLAY_DATA_GRAY_TABLE[i]);
        #endif
    }
}

void SSD_Core::_writeRegister(
    const uint8_t cmd,
    uint8_t data
) {
	_spi.writeCommand8(cmd);

    #if defined(SSD_CMD_DATA_BIT)
	    _spi.writeData8(data);
    #else
	    _spi.writeCommand8(data);
    #endif
}

void SSD_Core::_drawHorizontalLine(
    int16_t x, 
    int16_t y, 
    int16_t w, 
    uint16_t color
) {
    if (w < 1) return;
    _setAddressWindow(x, y, x + w - 1, y, true);
    _spi.writeData16Multi(color, w);
}

void SSD_Core::_drawVerticalLine(
    int16_t x, 
    int16_t y, 
    int16_t h, 
    uint16_t color
) {
    if (h < 1) return;
    _setAddressWindow(x, y, x, y + h - 1, true);
    _spi.writeData16Multi(color, h);
}

void SSD_Core::_drawLine(
    int16_t x0, 
    int16_t y0, 
    int16_t x1, 
    int16_t y1, 
    uint16_t color
) {
    // Check for out of bounds.
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0) return;

    // SSD_1331 or SSD_1332:
	#if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)

        // Get delay for draw op.
		int dly = _calculateDelay(x1 - x0, y1 - y0, SSD_DISPLAY_DATA_LINE_DELAY);

        // Get color.
		uint8_t r,g,b;
		SSD_Util::_convertColor(color, r, g, b);

        // Draw line.
        if (_screenConfig.isPortrait()){
            swapVals(x0,y0);
            swapVals(x1,y1);
        }
        _spi.writeCommand8(CMD_DRAWLINE);
        _spi.writeCommand8(x0 & 0xFF);
        _spi.writeCommand8(y0 & 0xFF);
        _spi.writeCommand8(x1 & 0xFF);
        _spi.writeCommand8(y1 & 0xFF);
		_writeColorData(r, g, b);
		delayMicroseconds(dly);

    // SSD_1351:
	#else

        // Single pixel:
        if (x0 == x1 && y0 == y1) {
            _drawPixel(x0, y0, color);
            return;
        }

        // Horizontal line:
		if (y0 == y1) {
			if (x1 > x0) {
				_drawHorizontalLine(x0, y0, x1 - x0 + 1, color);
			} else {
				_drawHorizontalLine(x1, y0, x0 - x1 + 1, color);
			}
			return;
		} 
        
        // Vertical line:
        if (x0 == x1) {
			if (y1 > y0) {
				_drawVerticalLine(x0, y0, y1 - y0 + 1, color);
			} else {
				_drawVerticalLine(x0, y1, y0 - y1 + 1, color);
			}		
			return;
		}

		bool steep = abs(y1 - y0) > abs(x1 - x0);
		if (steep) {swapVals(x0, y0); swapVals(x1, y1);}
		if (x0 > x1) {swapVals(x0, x1); swapVals(y0, y1);}

		int16_t dx, dy;
		dx = x1 - x0;
		dy = abs(y1 - y0);

		int16_t err = dx / 2;
		int16_t ystep;

		if (y0 < y1) {
			ystep = 1;
		} else {
			ystep = -1;
		}

		int16_t xbegin = x0;
		if (steep) {
			for (; x0<=x1; x0++) {
				err -= dy;
				if (err < 0) {
					int16_t len = x0 - xbegin;
					if (len) {
						_drawVerticalLine(y0, xbegin, len + 1, color);
					} else {
						_drawPixel(y0, x0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) _drawVerticalLine(y0, xbegin, x0 - xbegin, color);
		} else {
			for (; x0<=x1; x0++) {
				err -= dy;
				if (err < 0) {
					int16_t len = x0 - xbegin;
					if (len) {
						_drawHorizontalLine(xbegin, y0, len + 1, color);
					} else {
						_drawPixel(x0, y0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) _drawHorizontalLine(xbegin, y0, x0 - xbegin, color);
		}
	#endif
}

#if defined(SSD_1331_REGISTERS_H) || \
    defined(SSD_1332_REGISTERS_H)
	
void SSD_Core::_setFillState(bool filling) {
    static bool _filled;
    if (filling != _filled){
        _filled = filling;
        _spi.writeCommand8(CMD_FILLMODE);
        if (filling){
            _spi.writeCommand8(0x01);
        } else {
            _spi.writeCommand8(0x00);
        }
    }
}

void SSD_Core::_writeColorData(
    uint8_t r,
    uint8_t g,
    uint8_t b
) {
    _spi.writeCommand8(r);
    _spi.writeCommand8(g);
    _spi.writeCommand8(b);
}
	
#endif

/* #endregion */
// ===================================================
