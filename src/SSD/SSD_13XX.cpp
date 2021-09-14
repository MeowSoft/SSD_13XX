/*=========================================================================================
	Part of SSD_13XX library
    Copyright (c) 2014/2015/2016, .S.U.M.O.T.O.Y., coded by Max MC Costa.

    SSD_13XX Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    SSD_13XX Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
===========================================================================================*/

#include "SSD_13XX.h"

#include "SSD_Util.h"



/* #region Transaction methods: */

void SSD_13XX::setAddressWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	_spi.startTransaction();
	_setAddressWindow(x0,y0,x1,y1,true);
	_spi.deselectAndEndTransaction();
}

void SSD_13XX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	//if (boundaryCheck(x,y)) return;
	//if ((x < 0) || (y < 0)) return;
	_spi.startTransaction();
	_drawPixel(x,y,color);
	_spi.deselectAndEndTransaction();
}

/*
draw LINE
*/
void SSD_13XX::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
    int16_t w = _screenConfig.getWidth();
    int16_t h = _screenConfig.getHeight();
	if (x1 >= w) x1 = w-1;
	if (y1 >= h) y1 = h-1;
	_spi.startTransaction();
	_drawLine(x0,y0,x1,y1,color/*,200*/);
	_spi.deselectAndEndTransaction();
}

/*
draw fast vertical line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void SSD_13XX::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	/*
	if (boundaryCheck(x,y)) return;
	if (((y + h) - 1) >= _height) h = _height - y;
	h = sizeCheck(y,h,_height);
	*/
	if (x >= _screenConfig.getWidth()-1) return;
	if (y >= _screenConfig.getHeight()-1) return;
	_spi.startTransaction();
	_drawVerticalLine(x,y,h,color);
	_spi.deselectAndEndTransaction();
}


/*
draw fast horizontal line
this uses fast contiguos commands method but opens SPi transaction and enable CS
then set CS hi and close transaction
*/
void SSD_13XX::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	/*
	if (boundaryCheck(x,y)) return;
	if (((x + w) - 1) >= _width)  w = _width - x;
	w = sizeCheck(x,w,_width);
	*/
	if (x >= _screenConfig.getWidth()-1) return;
	if (y >= _screenConfig.getHeight()-1) return;
	_spi.startTransaction();
	_drawHorizontalLine(x,y,w,color);
	_spi.deselectAndEndTransaction();
}

/*
fill RECT
*/
//+++++++++OK
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	_spi.startTransaction();
	_drawRectangle(x, y, w, h, color,color,true);
	_spi.deselectAndEndTransaction();
}

/*
fill RECT with gradient
*/
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2)
{
	if (!_checkBounds(x,y)) return;
	_spi.startTransaction();
	_drawRectangleWithGradient(x,y,w,h,color1,color2);
	_spi.deselectAndEndTransaction();
}


/*
draw rect
*/

//+++++++++OK
void SSD_13XX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	drawRect(x,y,w,h,color,color,false);
}

//+++++++++OK
void SSD_13XX::drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2,bool filled)
{
	if (x + w >= _screenConfig.getWidth()) return;
	if (y + h >= _screenConfig.getHeight()) return;
	_spi.startTransaction();
	_drawRectangle(x, y, w, h, color1,color2,filled);
	_spi.deselectAndEndTransaction();
}


/* #endregion */

/*********************************************************
********************** constructors **********************
**********************************************************/


SSD_13XX::SSD_13XX(SPI_Driver spi, const uint8_t rstPin) {
    _rstPin  = rstPin;
    _spi = spi;
}


/*********************************************************
************** Var init and SPI preparation **************
**********************************************************/
void SSD_13XX::begin()
{
//initialize Vars
_screenConfig.init(this);

	setColorDepth(SSD_COLORDEPTH);
	setColorOrder(SSD_RGBORDER);

	if (_rstPin != 255) {
		pinMode(_rstPin, OUTPUT);
		digitalWrite(_rstPin, HIGH);
		delay(10);
		digitalWrite(_rstPin, LOW);
		delay(10);
		digitalWrite(_rstPin, HIGH);
		delay(10);
	}
	/* -----------------------------------------------------------
	------------------- Chip Initialization ----------------------
	-------------------------------------------------------------*/
	delay(30);
	_spi.startTransaction();
	#if defined(_SSD_1331_96X64_H) || defined(_SSD_1332_96X64_H)
		
		_spi.writeCommand8(CMD_DISPLAYOFF);
		_writeRegister(CMD_CLOCKDIV,SSD_CLOCKDIV);
		_writeRegister(CMD_SETMULTIPLEX,SSD_SETMULTIPLEX);
		_writeRegister(CMD_STARTLINE,SSD_STARTLINE);
		_writeRegister(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		// _writeRegister(CMD_PHASEPERIOD,SSD_PHASEPERIOD);
		_writeRegister(CMD_SETCONFIG,SSD_SETMASTER);
		_writeRegister(CMD_POWERMODE,SSD_POWERMODE);
		_writeRegister(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		#if defined(SSD_GAMMASET)
			_spi.writeCommand8(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){_spi.writeCommand8(SSD_GRAYTABLE[i]);}
		#else
			_spi.writeCommand8(CMD_LINEARGRAY);
		#endif
		_writeRegister(CMD_CONTRASTA,SSD_CONTRAST_A);
		_writeRegister(CMD_CONTRASTB,SSD_CONTRAST_B);
		_writeRegister(CMD_CONTRASTC,SSD_CONTRAST_C);
		#if !defined(_SSD_1331_96X64_H)
		_writeRegister(CMD_VPACOLORLVL,SSD_VPACOLORLVL);
		_writeRegister(CMD_VPBCOLORLVL,SSD_VPBCOLORLVL);
		_writeRegister(CMD_VPCCOLORLVL,SSD_VPCCOLORLVL);
		#else
			_spi.writeCommand8(CMD_DIMMODESET);
			_spi.writeCommand8(0);
			_spi.writeCommand8(SSD_DIMMDESET_A);
			_spi.writeCommand8(SSD_DIMMDESET_B);
			_spi.writeCommand8(SSD_DIMMDESET_C);
			_spi.writeCommand8(SSD_DIMMDESET_PC);
			_writeRegister(CMD_PHASEPERIOD,SSD_PRECHARGE);
			_writeRegister(CMD_PRECHARGEA,SSD_PRECHARGE_A);
			_writeRegister(CMD_PRECHARGEB,SSD_PRECHARGE_B);
			_writeRegister(CMD_PRECHARGEC,SSD_PRECHARGE_C);
			_writeRegister(CMD_PRECHARGELEVEL,SSD_PRECHARGELEVEL);
		#endif
		_writeRegister(CMD_VCOMH,SSD_VCOMH);
		_setFillState(1);
	#elif defined(_SSD_1351_96X64_H) || defined(_SSD_1351_128X96_H) || defined(_SSD_1351_128X128_H)
		if (SSD_COMSPLIT == 1){
			_remapReg |= ((1 << 5));
		} else {
			_remapReg |= ((0 << 5));
		}
		_writeRegister(CMD_CMDLOCK,SSD_COMMANDLOCK1);
		_writeRegister(CMD_CMDLOCK,SSD_COMMANDLOCK2);
		_spi.writeCommand8(CMD_DISPLAYOFF);
		_writeRegister(CMD_CLOCKDIV,SSD_CLOCKDIV);
		_writeRegister(CMD_MUXRATIO,SSD_MUXRATIO);
		_writeRegister(CMD_STARTLINE,SSD_STARTLINE);
		_writeRegister(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		_writeRegister(CMD_SETGPIO,SSD_SETGPIO);
		_writeRegister(CMD_FUNCTIONSELECT,SSD_FUNCTIONSELECT);
		_spi.writeCommand8(CMD_SETVSL);
		_spi.writeData8(SSD_SETVSL_A);_spi.writeData8(SSD_SETVSL_B);_spi.writeData8(SSD_SETVSL_C);
		_spi.writeCommand8(CMD_CONTRASTABC);
		_spi.writeData8(SSD_CONTRAST_A);_spi.writeData8(SSD_CONTRAST_B);_spi.writeData8(SSD_CONTRAST_C);
		_writeRegister(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		_spi.writeCommand8(CMD_DISPLAYENHANCE);
		if (SSD_ENHANCE){
			_spi.writeData8(0xA4);
		} else {
			_spi.writeData8(0x00);
		}
		_spi.writeData8(0x00);
		_spi.writeData8(0x00);
		#if defined(SSD_GAMMASET)
			//_spi.writeCommand8(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){_spi.writeData8(SSD_GRAYTABLE[i]);}
		#else
			_spi.writeCommand8(CMD_USELUT);
		#endif
		// phase here
		_writeRegister(CMD_PRECHARGE,SSD_PRECHARGE);
		_writeRegister(CMD_PRECHARGE2,SSD_PRECHARGE2);
		_writeRegister(CMD_VCOMH,SSD_VCOMH);
	#endif
	//_setAddressWindow(0,0,SSD_WIDTH-1,SSD_HEIGHT-1,false);// ???
	//_spi.writeData16Multi(_defaultBgColor, SSD_CGRAM);//???
	//Normal Display and turn ON
	_spi.writeCommand8(CMD_NORMALDISPLAY);
	_spi.writeCommand8AndDeselect(CMD_DISPLAYON);
	_spi.endTransaction();
	delay(60);
	setRotation(SSD_ScreenConfig::ROTATION_LANDSCAPE);
	fillScreen(BLACK);

	delay(1);
	changeMode(SSD_ScreenConfig::NORMAL);
}



void SSD_13XX::setBrightness(uint8_t brightness)
{
	
	if (brightness > 15) brightness = 15;
	_spi.startTransaction();
		_spi.writeCommand8(CMD_MASTERCURRENT);
		#if defined(_SSD_USECMDASDATA)
			_spi.writeCommand8AndDeselect(brightness);
		#else
			_spi.writeData8AndDeselect(brightness);
		#endif
	_spi.endTransaction();
}

/*********************************************************
***************** Basic display commands *****************
**********************************************************/

/*
This change the mode of the display as:
	NORMAL: Normal mode.
	PWRSAVE: Consume much less power
	PROTECT: Display protect his serial comm, accept only a change mode as normal to exit protected state
	INVERT: It invert the display
	DISP_ON: Turn on display (if off) and enable backlight pin if used
	DISP_DIM: The all display goe dim
	DISP_OFF: The opposite of above
*/
void SSD_13XX::changeMode(const enum SSD_ScreenConfig::ScreenModes m)
{
	_screenConfig.changeMode(m);
}

uint8_t SSD_13XX::getMode(void)
{
	return _screenConfig.getMode();
}


void SSD_13XX::copyArea(int16_t sx0, int16_t sy0, int16_t sx1, int16_t sy1,int16_t dx, int16_t dy)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_screenConfig.isPortrait()){//not tested yet
		swapVals(sx0,sy0);
		swapVals(sx1,sy1);
		swapVals(dx,dy);
	}
	_spi.startTransaction();
	_spi.writeCommand8(CMD_DRAWCOPY);
		_spi.writeCommand8(sx0 & 0xFF);
		_spi.writeCommand8(sy0 & 0xFF);
		_spi.writeCommand8(sx1 & 0xFF);
		_spi.writeCommand8(sy1 & 0xFF);
		_spi.writeCommand8(dx & 0xFF);
		_spi.writeCommand8AndDeselect(dy & 0xFF);
	_spi.endTransaction();
	#endif
}

void SSD_13XX::dimArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_screenConfig.isPortrait()){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	_spi.startTransaction();
	_spi.writeCommand8(CMD_DIMWINDOW);
		_spi.writeCommand8(x0 & 0xFF);
		_spi.writeCommand8(y0 & 0xFF);
		_spi.writeCommand8(x1 & 0xFF);
		_spi.writeCommand8AndDeselect(y1 & 0xFF);
	_spi.endTransaction();
	#endif
}

void SSD_13XX::moveArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_screenConfig.isPortrait()){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	_spi.startTransaction();
	_spi.writeCommand8(CMD_CLRWINDOW);
		_spi.writeCommand8(x0 & 0xFF);
		_spi.writeCommand8(y0 & 0xFF);
		_spi.writeCommand8(x1 & 0xFF);
		_spi.writeCommand8AndDeselect(y1 & 0xFF);
	_spi.endTransaction();
	#endif
}

uint8_t SSD_13XX::getRotation(void)
{
	return _screenConfig.getRotation();
}

void SSD_13XX::setColorDepth(uint8_t depth)
{
	_screenConfig.setColorDepth(depth);
}

void SSD_13XX::setColorOrder(bool order)
{
	_screenConfig.setColorOrder(order);
}

//+++++++++OK
void SSD_13XX::setRotation(SSD_ScreenConfig::Rotations m)
{
    _screenConfig.setRotation(m);
    _screenConfig.writeRemap();


}

//+++++++++OK
int16_t SSD_13XX::width(void) const {
	return _screenConfig.getWidth();
}

//+++++++++OK
int16_t SSD_13XX::height(void) const {
	return _screenConfig.getHeight();
}

//+++++++++OK
int16_t SSD_13XX::cgWidth(void) const {
	return SSD_WIDTH;
}

//+++++++++OK
int16_t SSD_13XX::cgHeight(void) const {
	return SSD_HEIGHT;
}


/*********************************************************
************************   scroll  ***********************
**********************************************************/

/*
a:Set number of column as horizontal scroll offset Range: 0d-95d ( no horizontal scroll if equals to 0)
b:Define start row address
c:Set number of rows to be horizontal scrolled B[5:0]+C[6:0] <=64
d:Set number of row as vertical scroll offset Range: 0d-63d ( no vertical scroll if equals to 0)
e:Set time interval between each scroll step
*/
void SSD_13XX::defineScrollArea(int16_t a, int16_t b, int16_t c, int16_t d, uint8_t e)
{
	if (b+c > SSD_HEIGHT) return;
	uint8_t spd = 0;
	e = e % 4;
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
		if (e == 0){
			spd = 0b00000000;
		} else if (e == 1){
			spd = 0b00000001;
		} else if (e == 2){
			spd = 0b00000010;
		} else {
			spd = 0b00000011;
		}
		_spi.startTransaction();
		_spi.writeCommand8(CMD_SCROLL_SET);

		_spi.writeCommand8(a & 0xFF);
		_spi.writeCommand8(b & 0xFF);
		_spi.writeCommand8(c & 0xFF);
		_spi.writeCommand8(d & 0xFF);
		_spi.writeCommand8AndDeselect(spd);
		_spi.endTransaction();
	#elif defined(SSD_1351_REGISTERS_H)
		if (e == 1){
			spd |= ((1 << 0));
		} else if (e == 2){
			spd |= ((1 << 1));
		} else if (e > 2) {
			spd |= ((1 << 1) | (1 << 0));
		}
		_spi.startTransaction();
		_spi.writeCommand8(CMD_HORIZSCROLL);
		_spi.writeData8(0b00000001);//0b10000001
		_spi.writeData8(b & 0xFF);
		_spi.writeData8(c & 0xFF);
		_spi.writeData8(0);
		_spi.writeData8AndDeselect(spd);
		endTransaction();
	#endif
	
}


boolean SSD_13XX::scroll(bool active)
{
	_spi.startTransaction();
	if (active){
		_spi.writeCommand8AndDeselect(CMD_SCROLL_ON);
	} else {
		_spi.writeCommand8AndDeselect(CMD_SCROLL_OFF);
	}
	_spi.endTransaction();
	return active;
}


/*********************************************************
******************** Color Functions *********************
**********************************************************/


/*********************************************************
****************** Graphic Functions *********************
**********************************************************/
//+++++++++OK


//+++++++++OK
void SSD_13XX::fillScreen(uint16_t color)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
		uint8_t r1,g1,b1;
		SSD_Util::_convertColor(color,r1,g1,b1);
		_spi.startTransaction();
		_setFillState(1);
		_spi.writeCommand8(CMD_DRAWRECT);
		_spi.writeCommand16(0);
		_spi.writeCommand8(SSD_WIDTH-1);
		_spi.writeCommand8(SSD_HEIGHT-1);
		_writeColorData(r1,g1,b1);
		_writeColorData(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);//CMD_DLY_FILL
	#else
		_spi.startTransaction();
		_setAddressWindow(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
		);
		_spi.writeData16Multi(color, SSD_CGRAM);
	#endif
	_spi.deselectAndEndTransaction();
}



//with gradient
//+++++++++OK
void SSD_13XX::fillScreen(uint16_t color1,uint16_t color2)
{
	_spi.startTransaction();
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (color1 != color2){
		_drawRectangleWithGradient(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
	} else {
		uint8_t r1,g1,b1;
		SSD_Util::_convertColor(color1,r1,g1,b1);
		_setFillState(1);
		_spi.writeCommand8(CMD_DRAWRECT);
		_spi.writeCommand16(0);
		_spi.writeCommand8(SSD_WIDTH-1);
		_spi.writeCommand8(SSD_HEIGHT-1);
		_writeColorData(r1,g1,b1);
		_writeColorData(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);
	}
	#else
		if (color1 != color2){
			drawRectangleWithGradient(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
		} else {
			_setAddressWindow(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
			);
			_spi.writeData16Multi(color1, SSD_CGRAM);
		}
	#endif
	_spi.deselectAndEndTransaction();
}

//+++++++++OK
void SSD_13XX::clearScreen(void)
{
	fillScreen(BLACK);
	//_cursorX = _cursorY = 0;
}








void SSD_13XX::_writeRegister(
    const uint8_t cmd,
    uint8_t data
) {
	_spi.writeCommand8(cmd);
	#if defined(_SSD_USECMDASDATA)
	_spi.writeCommand8(data);
	#else
	_spi.writeData8(data);
	#endif
}

void SSD_13XX::_drawHorizontalLine(
    int16_t x, 
    int16_t y, 
    int16_t w, 
    uint16_t color
) {
    if (w < 1) return;
    _setAddressWindow(x, y, x + w - 1, y, true);
    _spi.writeData16Multi(color, w);
}

void SSD_13XX::_drawVerticalLine(
    int16_t x, 
    int16_t y, 
    int16_t h, 
    uint16_t color
) {
    if (h < 1) return;
    _setAddressWindow(x, y, x, y + h - 1, true);
    _spi.writeData16Multi(color, h);
}

void SSD_13XX::_drawLine(
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
		int dly = _calculateDelay(x1 - x0, y1 - y0, CMD_DLY_LINE);

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
	
void SSD_13XX::_setFillState(bool filling)
{
    static bool _filled;
    if (filling != _filled){
        _filled = filling;
        _spi.writeCommand8(CMD_FILL);
        if (filling){
            _spi.writeCommand8(0x01);
        } else {
            _spi.writeCommand8(0x00);
        }
    }
}

void SSD_13XX::_writeColorData(
    uint8_t r,
    uint8_t g,
    uint8_t b
) {
    _spi.writeCommand8(r);
    _spi.writeCommand8(g);
    _spi.writeCommand8(b);
}
	
#endif






