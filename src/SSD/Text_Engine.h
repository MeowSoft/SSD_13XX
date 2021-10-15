#ifndef TEXT_ENGINE_H
#define TEXT_ENGINE_H

#include <stdint.h>
#include "Print.h"

#include "SSD_13XX.h"
#include "_includes/_cpuCommons.h"
#include "_includes/sumotoy_fontDescription.h"

#define _SSD_DEF_FONT_PATH			"_fonts/defaultFont.c"
	#define _SSD_DEF_FONT_NAME			defaultFont

#if defined(_SSD_DEF_FONT_PATH)
	#include _SSD_DEF_FONT_PATH
#else
	#include "_fonts/nullfont.c"
#endif
#if !defined(CENTER)
#define CENTER 				9998
#endif


#include "_includes/_common_16bit_colors.h"
//#include "_settings/SSD_13XX_settings.h"
#include "_includes/sumotoy_imageDescription.h"
#include "_includes/sumotoy_iconDescription.h"
enum SSD_13XX_centerMode{NORM=0,SCREEN,REL_X,REL_Y,REL_XY};


class Text_Engine : public Print 
{


public:

    void begin(SSD_13XX* ssd);
    void resetCursor();
    void setFont(const tFont *font);
    void setInternalFont(void);
    void setFontInterline(uint8_t distance);
    void setCharSpacing(uint8_t space);
    void setTextWrap(bool w);
    void setTextColor(uint16_t frgrnd, uint16_t bckgnd);
    void setTextColor(uint16_t color);
    void setTextScale(uint8_t sx,uint8_t sy);
    void setTextScale(uint8_t s);
    void getCursor(int16_t &x, int16_t &y);
    void setCursor(int16_t x, int16_t y,enum SSD_13XX_centerMode c=NORM);
    void 		drawIcon(int16_t x, int16_t y,const tIcon *icon,uint8_t scale=1,uint16_t f=WHITE,uint16_t b=BLACK,bool inverse=false);
   virtual size_t 	write(uint8_t b) { _textWrite((const char *)&b, 1); return 1;}
	virtual size_t  write(const uint8_t *buffer, size_t size) {_textWrite((const char *)buffer, size); return size;}

private:

    SSD_13XX* ssd_;
	uint8_t					_centerText;
	volatile int16_t		 _cursorX, _cursorY;
    uint16_t 				_textForeground;
	uint16_t 				_textBackground;
	uint8_t					_textScaleX;
	uint8_t					_textScaleY;
uint8_t					_charSpacing;
	uint8_t					_fontInterline;
	bool 				_textWrap; // If set, 'wrap' text at right edge of display
uint8_t					_fontRemapOffset;
/* fot remap offset will remap sections of char to other sections
		0: no remap
		1:minus to CAPITAL  if (c > 96 && c < 123) c -= 32;
		2:CAPITAl to minus  if (c > 64 && c < 91)  c += 32;
	*/
const tFont   		*	_currentFont;
int						_spaceCharWidth;

    void _charLineRender(
									bool 			lineBuffer[],
									int 			charW,
									int16_t 		x,
									int16_t 		y,
									uint8_t 		scaleX,
									uint8_t 		scaleY,
									int16_t 		currentYposition,
									uint16_t 		foreColor
									);

    void _glyphRender_unc(
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
									);

    bool _renderSingleChar(const char c);

    void _textWrite(const char* buffer, uint16_t len);

    int _STRlen_helper(const char* buffer,int len);
    int _getCharCode(uint8_t ch);

    // SSD interface methods: 
    bool boundaryCheck(int16_t xw,int16_t yh);
    int16_t getHeight();
    int16_t getWidth();
    bool isPortrait();
    void		setAddressWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
void 		drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled);
void 		fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2);
void 		setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix = true);

    void startTransaction();
    void closeTransaction();
};

#endif
