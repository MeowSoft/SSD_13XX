#include "SSD_ScreenConfig.h"
#include "SSD_Core.h"

void SSD_ScreenConfig::init(SSD_Core* ssd) {

    // Initialize width and height.
    _width = SSD_DISPLAY_DATA_WIDTH;
    _height = SSD_DISPLAY_DATA_HEIGHT;

    // Set rotation and screen mode.
    _rotation = ROTATION_LANDSCAPE;
	_currentMode = NORMAL;

    // Set SSD pointer.
    _ssd = ssd;

    // Initialize remap register.
    _remapReg = 0;

    // Set COM split mode.
    if (SSD_DISPLAY_DATA_HAS_COM_SPLIT){
        _remapReg |= ((1 << 5));
    } else {
        _remapReg |= ((0 << 5));
    }
}

void SSD_ScreenConfig::setColorDepth(uint8_t depth) {

    // 18 bit color depth. (1351 only)
    if (depth > 16) {
        #if defined(SSD_1351_REGISTERS_H)
        _remapReg |= ((1 << 7) | (0 << 6));
        return;
        #endif
    }

    // 16 bit color depth. (And 18 bit for 1331 and 1332)
    if (depth > 8) {
        _remapReg |= ((0 << 7) | (1 << 6));
        return;
    }

    // 8 bit color depth.
    _remapReg |= ((0 << 7) | (0 << 6));
}

void SSD_ScreenConfig::setColorOrder(bool useBGR) {
    #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
    _remapReg |= ((useBGR << 2));
    #endif
}

void SSD_ScreenConfig::changeMode(ScreenModes mode) {

    // If mode is already set, then bail.
	if (mode == _currentMode) return;

    // Otherwise, start SPI transaction.
	_ssd->_spi.startTransaction();

    // Select desired mode:
    switch(mode) {

        case NORMAL:

            // If display is off or dimmed, then turn it on.
            if (_currentMode == DISP_OFF || _currentMode == DISP_DIM) {
                _ssd->_spi.writeCommand8(CMD_DISPLAYON);
            }
        
            // If in power save mode, then turn off power save.
            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
            if (_currentMode == PWRSAVE) {
                _ssd->_writeRegister(CMD_POWERMODE, VAL_POWERMODE_OFF);
            }
            #endif
            
            // If in protect mode, then unlock.
            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
            if (_currentMode == PROTECT){
                _ssd->_writeRegister(CMD_LOCK, VAL_UNLOCK);
            }
            #endif
            
            // Set normal display mode.
            _ssd->_spi.writeCommand8(CMD_NORMALDISPLAY);
            _currentMode = NORMAL;

        break;

        case ALL_ON:

            _ssd->_spi.writeCommand8(CMD_DISPLAYALLON);
            _currentMode = ALL_ON;

        break;

        case ALL_OFF:

            _ssd->_spi.writeCommand8(CMD_DISPLAYALLOFF);
            _currentMode = ALL_OFF;

        break;

        case PWRSAVE:

            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
                _ssd->_spi.writeCommand8(CMD_POWERMODE);
                _ssd->_spi.writeCommand8AndDeselect(0x1A);
                _currentMode = PWRSAVE;
                delay(5);
            #endif

        break;

        case INVERT:

            _ssd->_spi.writeCommand8(CMD_INVERTDISPLAY);
            _currentMode = INVERT;

        break;

        case DISP_ON:

            _ssd->_spi.writeCommand8(CMD_DISPLAYON);
            _currentMode = DISP_ON;

        break;

        case DISP_OFF:

            _ssd->_spi.writeCommand8(CMD_DISPLAYOFF);
            _currentMode = DISP_OFF;

        break;

        case PROTECT:

            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
            _ssd->_writeRegister(CMD_LOCK, VAL_LOCK);
            _currentMode = PROTECT;
            #endif

        break;

        case DISP_DIM:

            _ssd->_spi.writeCommand8(CMD_DISPLAYDIM);

        break;

        default:
        break;
    }
    
    // Finish SPI transaction.
    _ssd->_spi.deselectAndEndTransaction();
}

void SSD_ScreenConfig::setRotation(Rotations rotation) {

    // Reset width and height.
    _width  = SSD_DISPLAY_DATA_WIDTH;
    _height = SSD_DISPLAY_DATA_HEIGHT;

    // Set rotation variable.
    _rotation = rotation;

    // Clear bits 0, 1, 3, and 4.
	_remapReg &= ~(0x1B);

    switch(rotation) {

        case ROTATION_LANDSCAPE:

            #if defined(SSD_1331_REGISTERS_H)
                _remapReg |= ((1 << 4) | (1 << 1));
            #else
                _remapReg |= ((1 << 4));
            #endif

        break;

        case ROTATION_PORTRAIT:

            swapVals(_width, _height);

            #if defined(SSD_1331_REGISTERS_H)
                _remapReg |= ((1 << 4) | (1 << 0));
            #else
                _remapReg |= ((1 << 4) | (1 << 1) | (1 << 0));
            #endif

        break;

        case ROTATION_LANDSCAPE_REVERSED:

            #if defined(SSD_1331_REGISTERS_H)
                // Bits 0, 1, 3, and 4 remain 0.
            #else
                _remapReg |= ((1 << 1));
            #endif

        break;

        case ROTATION_PORTRAIT_REVERSED:

            swapVals(_width, _height);

            #if defined(SSD_1331_REGISTERS_H)
                _remapReg |= ((1 << 1) | (1 << 0));
            #else
                _remapReg |= ((1 << 0));
            #endif

        break;
    }
}
    
void SSD_ScreenConfig::writeRemap(void) {

    // Start SPI transaction.
    _ssd->_spi.startTransaction();

    // Remap command.
	_ssd->_spi.writeCommand8(CMD_SETREMAP);

    // Write remap register.
    #if defined(SSD_CMD_DATA_BIT)
        _ssd->_spi.writeData8AndDeselect(_remapReg);
    #else
        _ssd->_spi.writeCommand8AndDeselect(_remapReg);
    #endif

    // Close transaction.
	_ssd->_spi.deselectAndEndTransaction();
}
