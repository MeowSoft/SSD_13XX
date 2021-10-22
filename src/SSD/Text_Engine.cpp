#include "Text_Engine.h"

bool Text_Engine::boundaryCheck(int16_t xw,int16_t yh) {
    return !ssd_->_checkBounds(xw, yh);
}
int16_t Text_Engine::getHeight() {
    return ssd_->_screenConfig.getHeight();
}
int16_t Text_Engine::getWidth() {
    return ssd_->_screenConfig.getWidth();
}
bool Text_Engine::isPortrait() {
    return ssd_->_screenConfig.isPortrait();
}
void Text_Engine::setAddressWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
    ssd_->_setAddressWindow(x0, y0, x1, y1);
}
void Text_Engine::drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled) {
    ssd_->_drawRectangle(x, y, w, h, color1, color2, filled);
}
void Text_Engine::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2) {
    ssd_->_drawGradient(x, y, w, h, color1, color2);
}
void Text_Engine::setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix) {
    ssd_->_setAddressWindow(x0, y0, x1, y1, rotFix);
}
void Text_Engine::startTransaction() {
    ssd_->_spi.startTransaction();
}
void Text_Engine::closeTransaction() {
    ssd_->_spi.deselectAndEndTransaction();
}

void Text_Engine::begin(SSD_Core* ssd) {
    ssd_ = ssd;
    _cursorY  		= _cursorX 		= 0;
	_textScaleX 	= _textScaleY 	= 1;
	_centerText 	= false;
	_fontInterline 	= 0;
	_charSpacing 	= 0;
	_textWrap      	= true;
    _textForeground = _textBackground = 0xffff;//text transparent
    setInternalFont();
}

void Text_Engine::resetCursor() {
    _cursorX = 0;
    _cursorY = 0;
}

void Text_Engine::setCursor(int16_t x, int16_t y,enum SSD_13XX_centerMode c)
{
	if (c == NORM){// No centering
		if (x == CENTER || y == CENTER) setCursor(x,y,SCREEN);
		if (boundaryCheck(x,y)) return;//cannot be
	} else if (c == SCREEN){// Absolute x,y or both center screen mode
		if (x == CENTER && y == CENTER) {//center x,y
			x = y = 0;//better reset since value is calculated in textWrite
			_centerText = 3;
		} else if (x == CENTER && y != CENTER) {//center on x
			if (y > getHeight()) return;//cannot be
			x = 0;//better reset since value is calculated in textWrite
			_centerText = 1;
		} else if (y == CENTER && x != CENTER) {//center on y
			if (x > getWidth()) return;//cannot be
			y = 0;//better reset since value is calculated in textWrite
			_centerText = 2;
		} else {
			setCursor(x,y,NORM);
		}
	} else {// Relative to x,y or both center mode
		if (x == CENTER || y == CENTER) setCursor(x,y,SCREEN);
		if (c == REL_X){//relative to X
			_centerText = 4;
		} else if (c == REL_Y){//relative to Y
			_centerText = 5;
		} else if (c == REL_XY){//relative XY
			_centerText = 6;
		}
	}
	if (isPortrait()) swapVals(x,y);
	_cursorX = x;
	_cursorY = y;
	setAddressWindow(0x0000,0x0000,x,y);
}

void Text_Engine::getCursor(int16_t &x, int16_t &y)
{
	x = _cursorX;
	y = _cursorY;
}


void Text_Engine::setTextScale(uint8_t s)
{
	_textScaleX = _textScaleY = (s > 0) ? s : 1;
}


void Text_Engine::setTextScale(uint8_t sx,uint8_t sy)
{
	_textScaleX = (sx > 0) ? sx : 1;
	_textScaleY = (sy > 0) ? sy : 1;
}

void Text_Engine::setTextColor(uint16_t color)
{
	_textForeground = _textBackground = color;
}

void Text_Engine::setTextColor(uint16_t frgrnd, uint16_t bckgnd)
{
	_textForeground = frgrnd;
	_textBackground = bckgnd;
}


void Text_Engine::setTextWrap(bool w)
{
	_textWrap = w;
}

void Text_Engine::setCharSpacing(uint8_t space)
{
	_charSpacing = space;
}

void Text_Engine::setFontInterline(uint8_t distance)
{
	_fontInterline = distance;
}

void Text_Engine::setInternalFont(void)
{
	#if defined(_SSD_DEF_FONT_PATH)
		setFont(&_SSD_DEF_FONT_NAME);
	#else
		//setFont(&nullfont);
	#endif
}


int Text_Engine::_getCharCode(uint8_t ch)
{
	int i;
	//remap ?
	if (_fontRemapOffset == 1 && (ch > 96 && ch < 123)){
		ch -= 32;
	} else if (_fontRemapOffset == 2 && ((ch > 64 && ch < 91))){
		ch += 32;
	}
	//search for char code
	for (i = 0;i < _currentFont->length;i++){
		if (_currentFont->chars[i].char_code == ch) return i;
	}
	return -1;
}


/*
	Return the lenght of a string in pixel with precision
*/
int Text_Engine::_STRlen_helper(const char* buffer,int len)
{
	int charIndex = -1;
	int i;
	//if (len < 1) len = strlen(buffer);		//try to get data from string
	//if (len < 1) return 0;					//better stop here
	if (_currentFont->font_widthType != 0){		// fixed width font
		return ((len * _spaceCharWidth));
	} else {								// variable width, need to loop trough entire string!
		int totW = 0;
		for (i = 0;i < len;i++){			//loop trough buffer
			if (buffer[i] == 32){			//a space
				totW += _spaceCharWidth;
			} else if (buffer[i] != 13 && buffer[i] != 10 && buffer[i] != 32){//avoid special char
				charIndex = _getCharCode(buffer[i]);
				if (charIndex > -1) {		//found!
					#if defined(_FORCE_PROGMEM__)
						totW += (pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width)));
					#else
						totW += (_currentFont->chars[charIndex].image->image_width);
					#endif
					totW += _charSpacing;
				}
			}//inside permitted chars
		}//buffer loop
		return totW;
	}//end variable w font
}


void Text_Engine::setFont(const tFont *font)
{
	_currentFont = font;
	_fontRemapOffset =  _currentFont->remap_code;
	//get all needed infos
	// We just get the space width now...
	int temp = _getCharCode(0x20);
	if (temp > -1){
		#if defined(_FORCE_PROGMEM__)
			_spaceCharWidth = pgm_read_byte(&(_currentFont->chars[temp].image->image_width));
		#else
			_spaceCharWidth = (_currentFont->chars[temp].image->image_width);
		#endif
	} else {
		//font malformed, doesn't have needed space parameter will return to system font
		setInternalFont();
		return;
	}
}

/*
Handle strings
*/
void Text_Engine::_textWrite(const char* buffer, uint16_t len)
{
	uint16_t i;
	if (len < 1) len = strlen(buffer);//try get the info from the buffer
	if (len < 1) return;//better stop here, the string it's really empty!
	// Center text flag enabled
	if (_centerText > 0){
		uint8_t stringWide = (_STRlen_helper(buffer,len) * _textScaleX) / 2;
		uint8_t strMidHeight = (((_currentFont->font_height - _currentFont->font_descent) * _textScaleY) / 2);
		if (_centerText < 4) {
			//absolute
			if (isPortrait()){
				if (_centerText > 1)  _cursorX = (getHeight() / 2) - strMidHeight;
				if (_centerText != 2) _cursorY = (getWidth() / 2) - stringWide;
			} else {
				if (_centerText > 1)  _cursorY = (getHeight() / 2) - strMidHeight;
				if (_centerText != 2) _cursorX = (getWidth() / 2) - stringWide;
			}
		} else {
			//relative
			if (isPortrait()){
				if (_centerText > 4)  _cursorX = _cursorX - strMidHeight;
				if (_centerText != 5) _cursorY = _cursorY - stringWide;
			} else {
				if (_centerText > 4)  _cursorY = _cursorY - strMidHeight;
				if (_centerText != 5) _cursorX = _cursorX - stringWide;
			}
		}
		if (_cursorX < 0)_cursorX = 0;
		if (_cursorY < 0)_cursorY = 0;
		_centerText = 0;//reset
	}//end center flag
	//Loop trough every char and write them one by one until end (or a break!)
	startTransaction();
	for (i=0;i<len;i++){
		if (_renderSingleChar(buffer[i])) {
			//aha! in that case I have to break out!
			break;
		}
		/*
	#if defined(ESP8266)
		yield();
	#endif
	*/
	}//end loop
	closeTransaction();
}

/*
Preprocessor for single chars
This function detect carriage/new line and space and perform separately.
When a char it's detected it pass the info to the drawChar function.
It return 0 most of the cases but can return 1 to inform the caller function to break
the string write loop.
*/
bool Text_Engine::_renderSingleChar(const char c)
{
	uint8_t borderRight = 0;
	uint8_t borderBottom = 0;
	if (c == 13){//------------------------------- CARRIAGE (detected) -----------------------
		return 0;//ignore, always OK
	} else if (c == 10){//------------------------- NEW LINE (detected) -----------------------
		borderBottom = (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
		if (!isPortrait()){
			//borderBottom = (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
			if (_cursorY + borderBottom  > getHeight()) return 1;//too high!
			_cursorX = 0;
			_cursorY += borderBottom;
		} else {//portrait
			//borderBottom = (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
			if (_cursorX + borderBottom  > getWidth()) return 1;//too high!
			_cursorX += borderBottom;
			_cursorY = 0;
		}
		return 0;
	} else if (c == 32){//--------------------------- SPACE (detected) -----------------------
		if (!isPortrait()){
			borderRight = (_spaceCharWidth * _textScaleX) + (_charSpacing * _textScaleX);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorX + borderRight >= getWidth()) borderRight = getWidth() - _cursorX;
				//fillRect_cont(
				drawRect_cont(
					_cursorX,
					_cursorY,
					borderRight + (_charSpacing * _textScaleX),
					(_currentFont->font_height * _textScaleY),
					_textBackground,
					_textBackground,
					1
				);
			}
			_cursorX += borderRight;
			return 0;
		} else {//portrait
			borderRight = (_spaceCharWidth * _textScaleY) + (_charSpacing * _textScaleY);
			if (_textForeground != _textBackground) {//fill the space
				if (_cursorY + borderRight >= getHeight()) borderRight = getHeight() - _cursorY;
				//fillRect_cont(
				drawRect_cont(
					_cursorY,
					_cursorX,
					borderRight,
					(_currentFont->font_height * _textScaleX),
					_textBackground,
					_textBackground,
					1
				);
			}
			_cursorY += borderRight;
			return 0;
		}
	} else {//-------------------------------------- CHAR  (detected) -------------------------
		int charIndex = _getCharCode(c);//get char code
		if (charIndex > -1){//check if it's valid
			int charW = 0;
			//I need to know the width...
			#if defined(_FORCE_PROGMEM__)
				charW = pgm_read_byte(&(_currentFont->chars[charIndex].image->image_width));
			#else
				charW = _currentFont->chars[charIndex].image->image_width;
			#endif
			//---------------------------------- WRAP is ON? --------------------------------
			if (_textWrap){//wrap, goes in the new line
				if (!isPortrait() && (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX)) >= getWidth()){
					_cursorX = 0;
					_cursorY += (_currentFont->font_height * _textScaleY) + (_fontInterline * _textScaleY);
				} else if (isPortrait() && (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY)) >= getWidth()){
					_cursorX += (_currentFont->font_height * _textScaleX) + (_fontInterline * _textScaleX);
					_cursorY = 0;
				}
			} else {//not wrap, will get rid of the data
				if (isPortrait()){
					if (_cursorY + (charW * _textScaleY) + (_charSpacing * _textScaleY) >= getWidth()) return 1;
				} else {
					if (_cursorX + (charW * _textScaleX) + (_charSpacing * _textScaleX) >= getWidth()) return 1;
				}
			}
			//-------------------------Actual single char drawing here -----------------------------------
			//updated in 1.0p7
			#if defined(_FORCE_PROGMEM__)
				const _smCharType * charGlyp = PROGMEM_read(&_currentFont->chars[charIndex].image->data);//char data
				int		totalBytes = pgm_read_word(&(_currentFont->chars[charIndex].image->image_datalen));
			#else
				const _smCharType * charGlyp = _currentFont->chars[charIndex].image->data;
				int		totalBytes = _currentFont->chars[charIndex].image->image_datalen;
			#endif
			if (!isPortrait()){
				if (_cursorY + (_currentFont->font_height * _textScaleY) > getHeight()) return 1;//too high!
				_glyphRender_unc(
								charGlyp,
								_cursorX,
								_cursorY,
								charW,
								_currentFont->font_height,
								_textScaleX,
								_textScaleY,
								totalBytes,
								_charSpacing,
								_textForeground,
								_textBackground,
								false
				);
				_cursorX += (charW * _textScaleX) + (_charSpacing * _textScaleX);//add charW to total
			} else {
				if (_cursorX + (_currentFont->font_height * _textScaleX) > getWidth()) return 1;//too high!
				_glyphRender_unc(
								charGlyp,
								_cursorY,
								_cursorX,
								charW,
								_currentFont->font_height,
								_textScaleX,
								_textScaleY,
								totalBytes,
								_charSpacing,
								_textForeground,
								_textBackground,
								false
				);
				_cursorY += (charW * _textScaleX) + (_charSpacing * _textScaleY);//add charW to total
			}
			return 0;
		}//end valid
		return 0;
	}//end char
}

/*
 - LGPO - rendering engine, part 1 (GNU v3)
This is the draw char function (version for uncompressed font)
It detects blank and filled lines and render separately, this is the first
accelleration step of the unique (and currently under commercial licence) sumotoy render engine,
it's a variation of LPGO font render accelleration used in RA8875 (under GNU v3).
The lines are not blank or filled are passed to the grouping function that is the second part of the accelleration.
*/
void Text_Engine::_glyphRender_unc(
									const 		_smCharType *pixelsArray,
									int16_t 	x,
									int16_t 	y,
									int 		glyphWidth,
									int 		glyphHeight,
									uint8_t 	scaleX,
									uint8_t	 	scaleY,
									uint16_t 	totalBytes,
									uint8_t 	cspacing,
									uint16_t 	foreColor,
									uint16_t 	backColor,
									bool 		inverse
									)
{
	//start by getting some glyph data...
	int i;
	uint8_t temp = 0;
	//some basic variable...
	uint8_t currentXposition = 0;//the current position of the writing cursor in the x axis, from 0 to glyphWidth
	uint8_t currentYposition = 0;//the current position of the writing cursor in the y axis, from 0 to _FNTheight
	uint8_t tempYpos = 0;
	uint16_t currentByte = 0;//the current byte in reading (from 0 to totalBytes)
	bool lineBuffer[glyphWidth+1];//the temporary line buffer
	int lineChecksum = 0;//part of the optimizer
	//Fill background if needed.
	//if (foreColor != backColor) fillRect_cont(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,backColor);
	if (foreColor != backColor) drawRect_cont(x,y,((glyphWidth * scaleX) + (cspacing * scaleX)),(glyphHeight * scaleY),backColor,backColor,1);
	//the main loop that will read all bytes of the glyph
	while (currentByte < totalBytes){
		//delay(1);
		//read n byte
		#if defined(_FORCE_PROGMEM__)
			temp = pgm_read_byte(&(pixelsArray[currentByte]));
		#else
			temp = pixelsArray[currentByte];
		#endif
		if (inverse) temp = ~temp;//invert byte if needed
		//read current bits inside current byte
		for (i = 7; i >= 0; i--){
			lineBuffer[currentXposition] = bitRead(temp,i);//continue fill line buffer
			lineChecksum += lineBuffer[currentXposition++];
			//----------------------------------- exception
			if (currentXposition == glyphWidth){
				//line buffer has been filled!
				currentXposition = 0;//reset the line x position
				tempYpos = y + (currentYposition * scaleY);
				if (lineChecksum < 1){
					//do nothing
				} else if (lineChecksum == glyphWidth){
					//full line
					//drawRect_cont(
					fillRect_cont(
							x,
							tempYpos,
							(glyphWidth * scaleX),
							scaleY,
							foreColor,
							backColor//,//backColor
							//1
					);
				} else {
					//line render
					_charLineRender(
							lineBuffer,
							glyphWidth,
							x,
							y,
							scaleX,
							scaleY,
							currentYposition,
							foreColor
					);
				}
				currentYposition++;//next line
				lineChecksum = 0;//reset checksum
			}//end exception
		}//end reading single byte
		currentByte++;
	}
}

/*
 - LGPO - rendering engine, part 2 (GNU v3)
LPGO font render accelleration (GNU v3), part 2, pixel grouping.
the sumotoy proprietary line render engine, please do not steal
without author permission since there's currently some licence on it!
This function group pixels with same color and perform much less memory addressing
than any other similar function I ever seen.
Here has been used to avoid multiple memory addressing but can be inproved, the LPGO shines
where harware accelleration it's present but this chip has only direct memory access...
*/
void Text_Engine::_charLineRender(
									bool 			lineBuffer[],
									int 			charW,
									int16_t 		x,
									int16_t 		y,
									uint8_t 		scaleX,
									uint8_t 		scaleY,
									int16_t 		currentYposition,
									uint16_t 		foreColor
									)
{

	int xlinePos = 0;
	int px;
	uint8_t endPix = 0;
	bool refPixel = false;
	while (xlinePos < charW){
		refPixel = lineBuffer[xlinePos];//xlinePos pix as reference value for next pixels
		//detect and render concurrent pixels
		for (px = xlinePos;px <= charW;px++){
			if (lineBuffer[px] == lineBuffer[xlinePos] && px < charW){
				//grouping pixels with same val
				endPix++;
			} else {
				if (refPixel) {
						//fillRect_cont(
						drawRect_cont(
						x,
						y + (currentYposition * scaleY),
						(endPix * scaleX),
						scaleY,
						foreColor,
						foreColor,
						1
					);
				}
				//reset and update some vals
				xlinePos += endPix;
				x += endPix * scaleX;
				endPix = 0;
				break;//exit cycle for...
			}
		}
	}//while
}



void Text_Engine::drawIcon(int16_t x, int16_t y,const tIcon *icon,uint8_t scale,uint16_t f,uint16_t b,bool inverse)
{
	#if defined(SSD_1351_REGISTERS_H)
		if (isPortrait()) swapVals(x,y);
	#endif
	#if defined(_FORCE_PROGMEM__)
		const _smCharType * iconData 	= PROGMEM_read(&icon->data);
		uint8_t		iWidth				= pgm_read_byte(&(icon->image_width));	//AVR ok
		uint8_t		iHeight				= pgm_read_byte(&(icon->image_height)); //AVR ok
		uint16_t	datalen				= pgm_read_word(&(icon->image_datalen));//AVR ok
		//boolean	dataComp			= PROGMEM_read(&icon->image_comp);//not yet
	#else
		const _smCharType * iconData	= icon->data;//icon data
		uint8_t		iWidth				= icon->image_width;
		uint8_t		iHeight				= icon->image_height;
		uint16_t	datalen				= icon->image_datalen;
		//uint8_t		dataComp		= icon->image_comp;//not yet
	#endif

	if (iWidth < 1 || iHeight < 1) return;//cannot be
	if (scale < 1) scale = 1;
	if ((x + iWidth) * scale >= getWidth() || (y + iHeight) * scale >= getHeight()) return;//cannot be

	startTransaction();
	//LGPO Rendering (uncomp)
	if (!isPortrait()){
		setAddrWindow_cont(x,y,iWidth+x,iHeight+y,false);
	_glyphRender_unc(
					iconData,
					x,
					y,
					iWidth,
					iHeight,
					scale,
					scale,
					datalen,
					0,
					f,
					b,
					inverse
	);
	} else {
		//setAddrWindow_cont(y,x,iWidth+y,iHeight+x,false);
		setAddrWindow_cont(y,x,iWidth+y,iHeight+x,false);
	_glyphRender_unc(
					iconData,
					y,
					x,
					iWidth,
					iHeight,
					scale,
					scale,
					datalen,
					0,
					f,
					b,
					inverse
	);
	}
	closeTransaction();
}
