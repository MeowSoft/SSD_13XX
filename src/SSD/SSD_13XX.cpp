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

#include "Spi/Spi_Instance.h"

#include "SSD_Util.h"


/* #region Transaction methods: */

void SSD_13XX::setArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	startTransaction();
	setAddrWindow_cont(x0,y0,x1,y1,true);
	closeTransaction();
}

void SSD_13XX::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	//if (boundaryCheck(x,y)) return;
	//if ((x < 0) || (y < 0)) return;
	startTransaction();
	drawPixel_cont(x,y,color);
	closeTransaction();
}

/*
draw LINE
*/
void SSD_13XX::drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color)
{
	if (x1 >= _width) x1 = _width-1;
	if (y1 >= _height) y1 = _height-1;
	startTransaction();
	drawLine_cont(x0,y0,x1,y1,color/*,200*/);
	closeTransaction();
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
	if (x >= _width-1) return;
	if (y >= _height-1) return;
	startTransaction();
	drawFastVLine_cont(x,y,h,color);
	closeTransaction();
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
	if (x >= _width-1) return;
	if (y >= _height-1) return;
	startTransaction();
	drawFastHLine_cont(x,y,w,color);
	closeTransaction();
}

/*
fill RECT
*/
//+++++++++OK
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	startTransaction();
	drawRect_cont(x, y, w, h, color,color,true);
	closeTransaction();
}

/*
fill RECT with gradient
*/
void SSD_13XX::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2)
{
	if (boundaryCheck(x,y)) return;
	startTransaction();
	fillRect_cont(x,y,w,h,color1,color2);
	closeTransaction();
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
	if (x + w >= _width) return;
	if (y + h >= _height) return;
	startTransaction();
	drawRect_cont(x, y, w, h, color1,color2,filled);
	closeTransaction();
}


/* #endregion */

/*********************************************************
********************** constructors **********************
**********************************************************/


#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
        Serial.println("MINE");
		_rst  = rstpin;
        InitSpi(mosi, sclk, cspin, dcpin, CMD_NOP, true);
	}
#elif defined(__MKL26Z64__) //Teensy LC
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin,const uint8_t mosi,const uint8_t sclk)
	{
		Serial.println("MINE");
		_rst  = rstpin;
        InitSpi(mosi, sclk, cspin, dcpin, CMD_NOP, true);
	}
#else //All the rest
	SSD_13XX::SSD_13XX(const uint8_t cspin,const uint8_t dcpin,const uint8_t rstpin)
	{
        Serial.println("MINE");
		_rst  = rstpin;
        InitSpi(cspin, dcpin, true);
	}
#endif

/*********************************************************
************** Var init and SPI preparation **************
**********************************************************/
void SSD_13XX::begin(bool avoidSPIinit)
{
//initialize Vars
	_remapReg		= 0;
	_currentMode	= 0b00000000;
	_portrait 		= false;

	_initError 		= 0b00000000;

	setColorDepth(SSD_COLORDEPTH);
	setColorOrder(SSD_RGBORDER);
	_defaultBgColor = _SSD_DEF_BACKGROUND;
	_defaultFgColor = _SSD_DEF_FOREGROUND;

	if (_rst != 255) {
		pinMode(_rst, OUTPUT);
		digitalWrite(_rst, HIGH);
		delay(10);
		digitalWrite(_rst, LOW);
		delay(10);
		digitalWrite(_rst, HIGH);
		delay(10);
	}
	/* -----------------------------------------------------------
	------------------- Chip Initialization ----------------------
	-------------------------------------------------------------*/
	delay(30);
	startTransaction();
	#if defined(_SSD_1331_96X64_H) || defined(_SSD_1332_96X64_H)
		if (SSD_COMSPLIT == 1){
			_remapReg |= ((1 << 5));
		} else {
			_remapReg |= ((0 << 5));
		}
		writecommand_cont(CMD_DISPLAYOFF);
		setRegister_cont(CMD_CLOCKDIV,SSD_CLOCKDIV);
		setRegister_cont(CMD_SETMULTIPLEX,SSD_SETMULTIPLEX);
		setRegister_cont(CMD_STARTLINE,SSD_STARTLINE);
		setRegister_cont(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		// setRegister_cont(CMD_PHASEPERIOD,SSD_PHASEPERIOD);
		setRegister_cont(CMD_SETCONFIG,SSD_SETMASTER);
		setRegister_cont(CMD_POWERMODE,SSD_POWERMODE);
		setRegister_cont(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		#if defined(SSD_GAMMASET)
			writecommand_cont(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){writecommand_cont(SSD_GRAYTABLE[i]);}
		#else
			writecommand_cont(CMD_LINEARGRAY);
		#endif
		setRegister_cont(CMD_CONTRASTA,SSD_CONTRAST_A);
		setRegister_cont(CMD_CONTRASTB,SSD_CONTRAST_B);
		setRegister_cont(CMD_CONTRASTC,SSD_CONTRAST_C);
		#if !defined(_SSD_1331_96X64_H)
		setRegister_cont(CMD_VPACOLORLVL,SSD_VPACOLORLVL);
		setRegister_cont(CMD_VPBCOLORLVL,SSD_VPBCOLORLVL);
		setRegister_cont(CMD_VPCCOLORLVL,SSD_VPCCOLORLVL);
		#else
			writecommand_cont(CMD_DIMMODESET);
			writecommand_cont(0);
			writecommand_cont(SSD_DIMMDESET_A);
			writecommand_cont(SSD_DIMMDESET_B);
			writecommand_cont(SSD_DIMMDESET_C);
			writecommand_cont(SSD_DIMMDESET_PC);
			setRegister_cont(CMD_PHASEPERIOD,SSD_PRECHARGE);
			setRegister_cont(CMD_PRECHARGEA,SSD_PRECHARGE_A);
			setRegister_cont(CMD_PRECHARGEB,SSD_PRECHARGE_B);
			setRegister_cont(CMD_PRECHARGEC,SSD_PRECHARGE_C);
			setRegister_cont(CMD_PRECHARGELEVEL,SSD_PRECHARGELEVEL);
		#endif
		setRegister_cont(CMD_VCOMH,SSD_VCOMH);
		_fillUtility(1);
	#elif defined(_SSD_1351_96X64_H) || defined(_SSD_1351_128X96_H) || defined(_SSD_1351_128X128_H)
		if (SSD_COMSPLIT == 1){
			_remapReg |= ((1 << 5));
		} else {
			_remapReg |= ((0 << 5));
		}
		setRegister_cont(CMD_CMDLOCK,SSD_COMMANDLOCK1);
		setRegister_cont(CMD_CMDLOCK,SSD_COMMANDLOCK2);
		writecommand_cont(CMD_DISPLAYOFF);
		setRegister_cont(CMD_CLOCKDIV,SSD_CLOCKDIV);
		setRegister_cont(CMD_MUXRATIO,SSD_MUXRATIO);
		setRegister_cont(CMD_STARTLINE,SSD_STARTLINE);
		setRegister_cont(CMD_DISPLAYOFFSET,SSD_DISPLAYOFFSET);
		setRegister_cont(CMD_SETGPIO,SSD_SETGPIO);
		setRegister_cont(CMD_FUNCTIONSELECT,SSD_FUNCTIONSELECT);
		writecommand_cont(CMD_SETVSL);
		writedata8_cont(SSD_SETVSL_A);writedata8_cont(SSD_SETVSL_B);writedata8_cont(SSD_SETVSL_C);
		writecommand_cont(CMD_CONTRASTABC);
		writedata8_cont(SSD_CONTRAST_A);writedata8_cont(SSD_CONTRAST_B);writedata8_cont(SSD_CONTRAST_C);
		setRegister_cont(CMD_MASTERCURRENT,SSD_MASTERCURRENT);
		writecommand_cont(CMD_DISPLAYENHANCE);
		if (SSD_ENHANCE){
			writedata8_cont(0xA4);
		} else {
			writedata8_cont(0x00);
		}
		writedata8_cont(0x00);
		writedata8_cont(0x00);
		#if defined(SSD_GAMMASET)
			//writecommand_cont(CMD_GRAYSCALE); for (uint8_t i =0;i<32;i++){writedata8_cont(SSD_GRAYTABLE[i]);}
		#else
			writecommand_cont(CMD_USELUT);
		#endif
		// phase here
		setRegister_cont(CMD_PRECHARGE,SSD_PRECHARGE);
		setRegister_cont(CMD_PRECHARGE2,SSD_PRECHARGE2);
		setRegister_cont(CMD_VCOMH,SSD_VCOMH);
	#endif
	//setAddrWindow_cont(0,0,SSD_WIDTH-1,SSD_HEIGHT-1,false);// ???
	//_pushColors_cont(_defaultBgColor, SSD_CGRAM);//???
	//Normal Display and turn ON
	writecommand_cont(CMD_NORMALDISPLAY);
	writecommand_last(CMD_DISPLAYON);
	endTransaction();
	delay(60);
	setRotation(_SSD_DEF_ROTATION);
	fillScreen(_defaultBgColor);

	delay(1);
	changeMode(NORMAL);
}

void SSD_13XX::setRegister_cont(const uint8_t cmd,uint8_t data)
{
	writecommand_cont(cmd);
	#if defined(_SSD_USECMDASDATA)
		writecommand_cont(data);
	#else
		writedata8_cont(data);
	#endif
}


void SSD_13XX::setBrightness(uint8_t brightness)
{
	
	if (brightness > 15) brightness = 15;
	startTransaction();
		writecommand_cont(CMD_MASTERCURRENT);
		#if defined(_SSD_USECMDASDATA)
			writecommand_last(brightness);
		#else
			writedata8_last(brightness);
		#endif
	endTransaction();
}

/*********************************************************
***************** Basic display commands *****************
**********************************************************/
/*
A simple helper to detect some error in initialization.
Since this library doesn't use MISO it can detect only
very basic errors:
0b00000001: Wrong MOSI,SCLK pin (only for some MCU's)
0b00000010: Wrong CS pin (only for some MCU's)
0b10000000: Display NOT supported (Only for multi-instances)
*/
uint8_t SSD_13XX::getErrorCode(void)
{
	return _initError;
}

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
void SSD_13XX::changeMode(const enum SSD_13XX_modes m)
{
	if (m != _currentMode){
		startTransaction();
		switch(m){
			case NORMAL:
				if (_currentMode == 6) {//was in off display?
					writecommand_cont(CMD_DISPLAYON);
				}
				/*
				if (_currentMode == 2) {//was in idle?
					//writecommand_cont(CMD_IDLEOF);
				}
				*/
				
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
				if (_currentMode == 8) {//was in powerMode?
					setRegister_cont(CMD_POWERMODE,SSD_POWERMODE);
					//delay(120);//needed
				}
				#endif
				
				if (_currentMode == 4){//was inverted?
					//SSD1332 should need only CMD_NORMALDISPLAY!?!
					#if defined(SSD_1331_REGISTERS_H)
					//writecommand_cont(CMD_DINVOF);
					#endif
				}
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
				if (_currentMode == 9){//was in protect mode?
					setRegister_cont(CMD_CMDLOCK,0x12);//unlock
				}
				#endif
				/*
				if (_currentMode == 12 || _currentMode == 13){//all on or off?
					//nothing
				}
				*/
				writecommand_cont(CMD_NORMALDISPLAY);
				_currentMode = 0;
			break;
			case ALL_ON:
				writecommand_cont(CMD_DISPLAYALLON);
				_currentMode = 12;
			break;
			case ALL_OFF:
				writecommand_cont(CMD_DISPLAYALLOFF);
				_currentMode = 13;
			break;
			case PWRSAVE: //power mode ON
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
					writecommand_cont(CMD_POWERMODE);
					writecommand_last(0x1A);
					_currentMode = 8;
					delay(5);//needed
				#else
					//TODO: exist?
					//writedata8_last(0x1A);
				#endif
				endTransaction();
				return;
			case INVERT:
				writecommand_cont(CMD_INVERTDISPLAY);//OK
				_currentMode = 4;
			break;
			case DISP_ON:
				writecommand_cont(CMD_DISPLAYON);
				_currentMode = 5;
			break;
			case DISP_OFF:
				writecommand_cont(CMD_DISPLAYOFF);
				_currentMode = 6;
			break;
			/*
			case DISP_DIM:
				writecommand_cont(CMD_DISPLAYDIM);
				_currentMode = 7;
			break;
			*/
			case PROTECT:
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
				setRegister_cont(CMD_CMDLOCK,0x16);//lock
				_currentMode = 9;
				#else
					endTransaction();
					return;
				#endif
			break;
			default:
				endTransaction();
				return;
			break;
		}
		closeTransaction();
	}
}

uint8_t SSD_13XX::getMode(void)
{
	return _currentMode;
}


void SSD_13XX::copyArea(int16_t sx0, int16_t sy0, int16_t sx1, int16_t sy1,int16_t dx, int16_t dy)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_portrait){//not tested yet
		swapVals(sx0,sy0);
		swapVals(sx1,sy1);
		swapVals(dx,dy);
	}
	startTransaction();
	writecommand_cont(CMD_DRAWCOPY);
		writecommand_cont(sx0 & 0xFF);
		writecommand_cont(sy0 & 0xFF);
		writecommand_cont(sx1 & 0xFF);
		writecommand_cont(sy1 & 0xFF);
		writecommand_cont(dx & 0xFF);
		writecommand_last(dy & 0xFF);
	endTransaction();
	#endif
}

void SSD_13XX::dimArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_portrait){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	startTransaction();
	writecommand_cont(CMD_DIMWINDOW);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_last(y1 & 0xFF);
	endTransaction();
	#endif
}

void SSD_13XX::moveArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (_portrait){//not tested yet
		swapVals(x0,y0);
		swapVals(x1,y1);
	}
	startTransaction();
	writecommand_cont(CMD_CLRWINDOW);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_last(y1 & 0xFF);
	endTransaction();
	#endif
}

uint8_t SSD_13XX::getRotation(void)
{
	return _rotation;
}

void SSD_13XX::setColorDepth(uint8_t depth)
{
	if (depth == 16){
		_colorDepth = 16;
		_remapReg |= ((0 << 7) | (1 << 6));
	#if defined(SSD_1351_REGISTERS_H)
	} else if (depth == 18) {
		_colorDepth = 16;
		_remapReg |= ((1 << 7) | (0 << 6));
	#endif
	} else {
		_colorDepth = 8;
		_remapReg |= ((0 << 7) | (0 << 6));
	}
}

void SSD_13XX::setColorOrder(bool order)
{
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
		_remapReg |= ((order << 2));
	#endif
}

//+++++++++OK
void SSD_13XX::setRotation(uint8_t m)
{
	_rotation = m % 4; // can't be higher than 3
	_portrait = false;
	_remapReg &= ~(0x1B);//clear bit 0,1,3,4
	_width  = SSD_WIDTH;
	_height = SSD_HEIGHT;
	if (_rotation == 0){
		#if defined(SSD_1331_REGISTERS_H)
			_remapReg |= ((1 << 4) | (1 << 1));//bit 4 & 1
		#elif defined(SSD_1332_REGISTERS_H)
			_remapReg |= ((1 << 4));//bit 4
		#elif defined(SSD_1351_REGISTERS_H)
			_remapReg |= ((1 << 4));//(1)
		#else
			//TODO
		#endif
	} else if (_rotation == 1){
		#if defined(SSD_1331_REGISTERS_H)
			_remapReg |= ((1 << 4) | (1 << 0));//bit 4 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(SSD_1332_REGISTERS_H)
			_remapReg |= ((1 << 4) | (1 << 1) | (1 << 0));//bit 4 & 1 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(SSD_1351_REGISTERS_H)
			_remapReg |= ((1 << 4) | (1 << 1) | (1 << 0));//(2)
			swapVals(_width,_height);
			_portrait = true;
		#else
			//TODO
		#endif
	} else if (_rotation == 2){
		#if defined(SSD_1331_REGISTERS_H)
		#elif defined(SSD_1332_REGISTERS_H)
			_remapReg |= ((1 << 1));//bit 1
		#elif defined(SSD_1351_REGISTERS_H)
			_remapReg |= ((1 << 1));//(3)
		#else
			//TODO
		#endif
	} else {
		#if defined(SSD_1331_REGISTERS_H)
			_remapReg |= ((1 << 1) | (1 << 0));//bit 1 & 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(SSD_1332_REGISTERS_H)
			_remapReg |= ((1 << 0));//bit 0
			swapVals(_width,_height);
			_portrait = true;
		#elif defined(SSD_1351_REGISTERS_H)
			_remapReg |= ((1 << 0));//(0)
			swapVals(_width,_height);
			_portrait = true;
		#else
			//TODO
		#endif
	}
	//_cursorX = 0; _cursorY = 0;//always reset cursor
	startTransaction();
		setAddrWindow_cont(0,0,_height-1,_width-1,false);
		writecommand_cont(CMD_SETREMAP);//set remap
		#if defined(_SSD_USECMDASDATA)
			writecommand_last(_remapReg);
		#else
			writedata8_last(_remapReg);
		#endif
	endTransaction();
}

//+++++++++OK
int16_t SSD_13XX::width(void) const {
	return _width;
}

//+++++++++OK
int16_t SSD_13XX::height(void) const {
	return _height;
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
		startTransaction();
		writecommand_cont(CMD_SCROLL_SET);

		writecommand_cont(a & 0xFF);
		writecommand_cont(b & 0xFF);
		writecommand_cont(c & 0xFF);
		writecommand_cont(d & 0xFF);
		writecommand_last(spd);
		endTransaction();
	#elif defined(SSD_1351_REGISTERS_H)
		if (e == 1){
			spd |= ((1 << 0));
		} else if (e == 2){
			spd |= ((1 << 1));
		} else if (e > 2) {
			spd |= ((1 << 1) | (1 << 0));
		}
		startTransaction();
		writecommand_cont(CMD_HORIZSCROLL);
		writedata8_cont(0b00000001);//0b10000001
		writedata8_cont(b & 0xFF);
		writedata8_cont(c & 0xFF);
		writedata8_cont(0);
		writedata8_last(spd);
		endTransaction();
	#endif
	
}


boolean SSD_13XX::scroll(bool active)
{
	startTransaction();
	if (active){
		writecommand_last(CMD_SCROLL_ON);
	} else {
		writecommand_last(CMD_SCROLL_OFF);
	}
	endTransaction();
	return active;
}


/*********************************************************
******************** Color Functions *********************
**********************************************************/


//+++++++++OK
void SSD_13XX::setBackground(uint16_t color)
{
	_defaultBgColor = color;
}

//+++++++++OK
void SSD_13XX::setForeground(uint16_t color)
{
	_defaultFgColor = color;
}

//+++++++++OK
uint16_t SSD_13XX::getBackground(void)
{
	return _defaultBgColor;
}

//+++++++++OK
uint16_t SSD_13XX::getForeground(void)
{
	return _defaultFgColor;
}

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
		startTransaction();
		_fillUtility(1);
		writecommand_cont(CMD_DRAWRECT);
		writecommand16_cont(0);
		writecommand_cont(SSD_WIDTH-1);
		writecommand_cont(SSD_HEIGHT-1);
		_sendColor_cont(r1,g1,b1);
		_sendColor_cont(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);//CMD_DLY_FILL
	#else
		startTransaction();
		setAddrWindow_cont(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
		);
		_pushColors_cont(color, SSD_CGRAM);
	#endif
	closeTransaction();
}



//with gradient
//+++++++++OK
void SSD_13XX::fillScreen(uint16_t color1,uint16_t color2)
{
	startTransaction();
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	if (color1 != color2){
		fillRect_cont(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
	} else {
		uint8_t r1,g1,b1;
		SSD_Util::_convertColor(color1,r1,g1,b1);
		_fillUtility(1);
		writecommand_cont(CMD_DRAWRECT);
		writecommand16_cont(0);
		writecommand_cont(SSD_WIDTH-1);
		writecommand_cont(SSD_HEIGHT-1);
		_sendColor_cont(r1,g1,b1);
		_sendColor_cont(r1,g1,b1);
		delayMicroseconds(CMD_DLY_FILL);
	}
	#else
		if (color1 != color2){
			fillRect_cont(0,0,SSD_WIDTH,SSD_HEIGHT,color1,color2);
		} else {
			setAddrWindow_cont(
				0,
				0,
				SSD_WIDTH - 1,
				SSD_HEIGHT - 1,
				false
			);
			_pushColors_cont(color1, SSD_CGRAM);
		}
	#endif
	closeTransaction();
}

//+++++++++OK
void SSD_13XX::clearScreen(void)
{
	fillScreen(_defaultBgColor);
	//_cursorX = _cursorY = 0;
}




/* ========================================================================
					    Fast Common Graphic Primitives
   ========================================================================*/

void SSD_13XX::drawFastHLine_cont(int16_t x, int16_t y, int16_t w, uint16_t color)
{
		if (w < 1) return;
		setAddrWindow_cont(x, y, x + w - 1, y,true);
		do { writedata16_cont(color); } while (--w > 0);
}

void SSD_13XX::drawFastVLine_cont(int16_t x, int16_t y, int16_t h, uint16_t color)
{
		if (h < 1) return;
		setAddrWindow_cont(x, y, x, y + h - 1,true);
		do { writedata16_cont(color); } while (--h > 0);
}



void SSD_13XX::drawLine_cont(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
	if (x0 < 0 || y0 < 0 || x1 < 0 || y1 < 0) return;
	#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
		int dly = _dlyHelper(x1-x0,y1-y0,CMD_DLY_LINE);
		uint8_t r,g,b;
		SSD_Util::_convertColor(color,r,g,b);
		_sendLineData_cont(x0,y0,x1,y1);
		_sendColor_cont(r,g,b);
		delayMicroseconds(dly);
	#else
		if (y0 == y1) {
			if (x1 > x0) {
				drawFastHLine_cont(x0, y0, x1 - x0 + 1, color);
			} else if (x1 < x0) {
				drawFastHLine_cont(x1, y0, x0 - x1 + 1, color);
			} else {
				drawPixel_cont(x0, y0, color);
			}
			return;
		} else if (x0 == x1) {
			if (y1 > y0) {
				drawFastVLine_cont(x0, y0, y1 - y0 + 1, color);
			} else {
				drawFastVLine_cont(x0, y1, y0 - y1 + 1, color);
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
						drawFastVLine_cont(y0, xbegin, len + 1, color);
					} else {
						drawPixel_cont(y0, x0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) drawFastVLine_cont(y0, xbegin, x0 - xbegin, color);
		} else {
			for (; x0<=x1; x0++) {
				err -= dy;
				if (err < 0) {
					int16_t len = x0 - xbegin;
					if (len) {
						drawFastHLine_cont(xbegin, y0, len + 1, color);
					} else {
						drawPixel_cont(x0, y0, color);
					}
					xbegin = x0 + 1;
					y0 += ystep;
					err += dx;
				}
				#if defined(ESP8266)   	
					yield(); 	
				#endif
			}
			if (x0 > xbegin + 1) drawFastHLine_cont(xbegin, y0, x0 - xbegin, color);
		}
	#endif
}


#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
	
	void SSD_13XX::_sendLineData_cont(int16_t x0,int16_t y0,int16_t x1,int16_t y1)
	{
		if (_portrait){
			swapVals(x0,y0);
			swapVals(x1,y1);
		}
		writecommand_cont(CMD_DRAWLINE);
		writecommand_cont(x0 & 0xFF);
		writecommand_cont(y0 & 0xFF);
		writecommand_cont(x1 & 0xFF);
		writecommand_cont(y1 & 0xFF);
	}
	
	void SSD_13XX::_sendColor_cont(uint8_t r,uint8_t g,uint8_t b)
	{
		writecommand_cont(r);writecommand_cont(g);writecommand_cont(b);
	}
	
	void SSD_13XX::_sendColor_cont(uint16_t color)
	{
		uint8_t r,g,b;
		SSD_Util::_convertColor(color,r,g,b);
		writecommand_cont(r);writecommand_cont(g);writecommand_cont(b);
	}
	
	void SSD_13XX::_fillUtility(bool filling)
	{
        static bool _filled;
		if (filling != _filled){

			_filled = filling;
			writecommand_cont(CMD_FILL);
			if (filling){
				writecommand_cont(0x01);
			} else {
				writecommand_cont(0x00);
			}
        }
	}


#endif


	void SSD_13XX::setAddrWindow_cont(uint16_t x, uint16_t y)
	{
		setAddrWindow_cont(x,y,SSD_WIDTH,SSD_HEIGHT,false);
	}





