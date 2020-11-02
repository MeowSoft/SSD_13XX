
#include "SSD_13XX.h"
#include "Spi/Spi_Instance.h"

#include "SSD_Util.h"


#define PREFIX inline

PREFIX void SSD_13XX::drawPixel_cont(int16_t x, int16_t y, uint16_t color)
{
		setAddrWindow_cont(x, y, x + 1, y + 1,true);
		writedata16_cont(color);
}

	PREFIX int SSD_13XX::_dlyHelper(int16_t w,int16_t h,int maxDly)//in microseconds
	{
		if (w <= 0 || h <= 0) return CMD_DLY_MIN;
		return map(w*h,2,SSD_CGRAM,CMD_DLY_MIN,maxDly);
	}


	PREFIX bool SSD_13XX::boundaryCheck(int16_t xw,int16_t yh)
	{
		if ((xw >= _width) || (yh >= _height)) return true;
		return false;
	}

	PREFIX void SSD_13XX::setAddrWindow_cont(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,bool rotFix)
	{
		if (rotFix && _portrait){
			swapVals(x0, y0);
			swapVals(x1, y1);
		}
		#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
			writecommand_cont(CMD_SETCOLUMN); //Column
			writecommand_cont(x0); writecommand_cont(x1);
			writecommand_cont(CMD_SETROW); //Page
			writecommand_cont(y0); writecommand_cont(y1);
		#else
			writecommand_cont(CMD_SETCOLUMN); //Column
			writedata8_cont(x0); writedata8_cont(x1);
			writecommand_cont(CMD_SETROW); //Page
			writedata8_cont(y0); writedata8_cont(y1);
			
			writecommand_cont(CMD_WRITERAM);
		#endif
	}

PREFIX void SSD_13XX::drawRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2, bool filled)
	{

		#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
			if (w < 2 && h < 2) {
				drawPixel_cont(x,y,color1);
				return;
			}
			if (_portrait){
				swapVals(x, y);
				swapVals(w, h);
			}
			if (x >= _width || y >= _height) return;
			int dly = _dlyHelper(w,h,CMD_DLY_FILL);
			uint8_t r1,g1,b1,r2,g2,b2;
			SSD_Util::_convertColor(color1,r1,g1,b1);
			if (color1 != color2){
				SSD_Util::_convertColor(color2,r2,g2,b2);
			} else {
				r2 = r1;g2 = g1;b2 = b1;
			}
			_fillUtility(filled);
			writecommand_cont(CMD_DRAWRECT);
			writecommand_cont(x & 0xFF);
			writecommand_cont(y & 0xFF);
			if ((x + w - 1) >= SSD_WIDTH) {
				writecommand_cont((SSD_WIDTH-1) & 0xFF);
			} else {
				writecommand_cont(((x + w) - 1) & 0xFF);
			}
			if ((y + h - 1) >= SSD_HEIGHT) {
				writecommand_cont((SSD_HEIGHT-1) & 0xFF);
			} else {
				writecommand_cont(((y + h) - 1) & 0xFF);
			}
			_sendColor_cont(r1,g1,b1);
			_sendColor_cont(r2,g2,b2);
			delayMicroseconds(dly);
		#else
			if (filled){
				fillRect_cont(x,y,w,h,color1,color2);
			} else {
				//TODO Grandient? (and color2)
				drawFastHLine_cont(x, y, w, color1);
				drawFastHLine_cont(x, (y+h)-1, w, color1);
				drawFastVLine_cont(x, y, h, color1);
				drawFastVLine_cont((x+w)-1, y, h, color1);	
			}
		#endif
	}

PREFIX void SSD_13XX::fillRect_cont(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2)
{
	//uint16_t colorTemp;
	if (w < 2 && h < 2){ drawPixel_cont(x,y,color1); return; }
	if (h < 2) { drawFastHLine_cont(x,y,w,color1); return; }
	if (w < 2) { drawFastVLine_cont(x,y,h,color1); return; }
	setAddrWindow_cont(x,y,(x+w)-1,(y+h)-1,true);
	if (color1 != color2){
		uint16_t pos = 0;
		uint8_t r1,r2,g1,g2,b1,b2;
		SSD_Util::Color565ToRGB(color1,r1,g1,b1);
		SSD_Util::Color565ToRGB(color2,r2,g2,b2);
		float pos2;
		uint8_t rR,gG,bB;
		uint16_t tot = h;
		uint16_t wtemp = w;
		do {
			pos2 = (float)pos/tot;
			rR = (((1.0 - pos2) * r1) + (pos2 * r2));
			gG = (((1.0 - pos2) * g1) + (pos2 * g2));
			bB = (((1.0 - pos2) * b1) + (pos2 * b2));
			#if defined(__MK20DX128__) || defined(__MK20DX256__) || defined(__MK64FX512__) || defined(__MK66FX1M0__)
				do {
					writedata16_cont(SSD_Util::Color565(rR,gG,bB));
				} while (--wtemp > 0);
				wtemp = w;
			#else
				_pushColors_cont(SSD_Util::Color565(rR,gG,bB),wtemp);
			#endif
			pos++;
		} while (--h > 0);
	} else {
		_pushColors_cont(color1,w*h);
	}
}

#undef PREFIX
