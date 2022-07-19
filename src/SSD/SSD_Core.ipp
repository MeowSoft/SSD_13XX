#include "SSD_Core.h"
#include "SSD_Util.h"

#define PREFIX inline

PREFIX int SSD_Core::_calculateDelay(
    int16_t w,
    int16_t h,
    int maxDly
) {
    if (w <= 0 || h <= 0) return SSD_DISPLAY_DATA_MIN_DELAY;
    return map(
        w * h, 
        2, 
        SSD_DISPLAY_DATA_WIDTH * SSD_DISPLAY_DATA_HEIGHT, 
        SSD_DISPLAY_DATA_MIN_DELAY, 
        maxDly
    );
}

PREFIX bool SSD_Core::_checkBounds(
    int16_t x,
    int16_t y
) {
    uint16_t w = _screenConfig.getWidth();
    uint16_t h = _screenConfig.getHeight();
    return (x >= 0 && y >= 0 && x < w && y < h);
}

PREFIX void SSD_Core:: _coerceBounds(
    int16_t& x,
    int16_t& y
) {
    uint16_t w = _screenConfig.getWidth();
    uint16_t h = _screenConfig.getHeight();
    if (x < 0) x = 0;
    if (x >= w) x = w - 1;
    if (y < 0) y = 0;
    if (y >= h) y = h - 1;
}

PREFIX void SSD_Core::_setAddressWindow(
    uint16_t rowStart, 
    uint16_t columnStart, 
    uint16_t rowEnd, 
    uint16_t columnEnd,
    bool checkRotation
) {
    if (checkRotation && _screenConfig.isPortrait()){
        swapVals(rowStart, columnStart);
        swapVals(rowEnd, columnEnd);
    }

    #if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)
        _spi.writeCommand8(CMD_SETCOLUMN);
        _spi.writeCommand8(rowStart); 
        _spi.writeCommand8(rowEnd);
        _spi.writeCommand8(CMD_SETROW);
        _spi.writeCommand8(columnStart);
        _spi.writeCommand8(columnEnd);
    #else
        _spi.writeCommand8(CMD_SETCOLUMN);
        _spi.writeData8(rowStart); 
        _spi.writeData8(rowEnd);
        _spi.writeCommand8(CMD_SETROW);
        _spi.writeData8(columnStart); 
        _spi.writeData8(columnEnd);
        _spi.writeCommand8(CMD_WRITERAM);
    #endif
}

PREFIX void SSD_Core::_drawPixel(
    int16_t x, 
    int16_t y, 
    uint16_t color
) {
    _setAddressWindow(x, y, x + 1, y + 1, true);
    _spi.writeData16(color);
}

PREFIX void SSD_Core::_drawRectangle(
    int16_t x, 
    int16_t y, 
    int16_t w, 
    int16_t h, 
    uint16_t color1,
    uint16_t color2, 
    bool filled
) {

    // For a 1 pixel size rectangle...
    if (w < 2 && h < 2) {

        // Draw the pixel and we're done.
        _drawPixel(x,y,color1);
        return;
    }

    // For a 1 pixel tall rectangle...
	if (h < 2) { 

        // Draw a line and we're done.
        _drawHorizontalLine(x, y, w, color1); 
        return; 
    }

    // For a 1 pixel wide rectangle...
	if (w < 2) { 

        // Draw a line and we're done.
        _drawVerticalLine(x, y, h, color1); 
        return; 
    }

    // SSD_1331 or SSD_1332:
	#if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)

    // Otherwise, swap x and y for portrait mode.
    if (_screenConfig.isPortrait()){
        swapVals(x, y);
        swapVals(w, h);
    }

    // If rectangle is out of screen bounds then bail.
	if (!_checkBounds(x, y)) return;

	// Get draw delay.
    int dly = _calculateDelay(w, h, SSD_DISPLAY_DATA_FILL_DELAY);

    // Get colors as individual RGB values.
	uint8_t r1, g1, b1, r2, g2, b2;
    SSD_Util::_convertColor(color1, r1, g1, b1);
    if (color1 != color2){
        SSD_Util::_convertColor(color2, r2, g2, b2);
    } else {
        r2 = r1; 
        g2 = g1; 
        b2 = b1;
    }

    // Get rectangle end x value or max screen width.
    int16_t endX = ((x + w - 1) >= SSD_DISPLAY_DATA_WIDTH) 
        ? (SSD_DISPLAY_DATA_WIDTH - 1)
        : ((x + w) - 1);

    // Get rectangle end y value or max screen height.
    int16_t endY = ((y + h - 1) >= SSD_DISPLAY_DATA_HEIGHT) 
        ? (SSD_DISPLAY_DATA_HEIGHT - 1) 
        : ((y + h) - 1);

	// Set fill state.
    _setFillState(filled);

    // Send commands to draw the rectangle.
    _spi.writeCommand8(CMD_DRAWRECT);
    _spi.writeCommand8(x & 0xFF);
    _spi.writeCommand8(y & 0xFF);
    _spi.writeCommand8(endX & 0xFF);
    _spi.writeCommand8(endY & 0xFF);
    _writeColorData(r1, g1, b1);
    _writeColorData(r2, g2, b2);
    delayMicroseconds(dly);

    // SSD_1351:
    #else
    if (filled){
        _drawGradient(x, y, w, h, color1, color2);
    } else {
        //TODO Grandient? (and color2)
        _drawHorizontalLine(x, y, w, color1);
        _drawHorizontalLine(x, (y + h) - 1, w, color1);
        _drawVerticalLine(x, y, h, color1);
        _drawVerticalLine((x + w) - 1, y, h, color1);	
    }
    #endif
}

PREFIX void SSD_Core::_drawGradient(
    int16_t x, 
    int16_t y, 
    int16_t w, 
    int16_t h, 
    uint16_t color1,
    uint16_t color2
) {

    // For a 1 pixel size rectangle...
    if (w < 2 && h < 2) {

        // Draw the pixel and we're done.
        _drawPixel(x, y, color1);
        return;
    }

    // For a 1 pixel tall rectangle...
	if (h < 2) { 

        // Draw a line and we're done.
        _drawHorizontalLine(x, y, w, color1); 
        return; 
    }

    // For a 1 pixel wide rectangle...
	if (w < 2) { 

        // Draw a line and we're done.
        _drawVerticalLine(x, y, h, color1); 
        return; 
    }

    // Otherwise, set address window for rectangle size.
	_setAddressWindow(x, y, (x + w) - 1, (y + h) - 1, true);

    // If colors are the same... 
    if (color1 == color2) {
        
        //Then we can just draw a solid rectangle and then we're done.
		_spi.writeData16Multi(color1, w * h);
        return;
    }

    // Otherwise, get color data.
    uint8_t r1, r2, g1, g2, b1, b2;
    SSD_Util::Color565ToRGB(color1, r1, g1, b1);
    SSD_Util::Color565ToRGB(color2, r2, g2, b2);

    float pos;
    uint8_t rR, gG, bB;
    uint16_t rowCount = 0;

    // For each row of pixels in the rectangle...
    do {

        // Adjust color according to row count.
        pos = (float)rowCount/h;
        rR = (((1.0 - pos) * r1) + (pos * r2));
        gG = (((1.0 - pos) * g1) + (pos * g2));
        bB = (((1.0 - pos) * b1) + (pos * b2));
    
        // Write color data.
        _spi.writeData16Multi(SSD_Util::Color565(rR, gG, bB), w);
        
    // Increment row count.
    } while (++rowCount < h);
}

#undef PREFIX
