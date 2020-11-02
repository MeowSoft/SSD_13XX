#include "SSD_Util.h"
#include <Arduino.h> // For sin, cos, and PI

#define PREFIX inline

PREFIX uint16_t SSD_Util::Color565(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }

	PREFIX uint16_t SSD_Util::Color24To565(int32_t color_) { 
        return ((((color_ >> 16) & 0xFF) / 8) << 11) | ((((color_ >> 8) & 0xFF) / 4) << 5) | (((color_) &  0xFF) / 8);
        }

	PREFIX uint16_t SSD_Util::htmlTo565(int32_t color_) { 
        return (uint16_t)(((color_ & 0xF80000) >> 8) | ((color_ & 0x00FC00) >> 5) | ((color_ & 0x0000F8) >> 3));
        }

	PREFIX void 	SSD_Util::Color565ToRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b){
        r = (((color & 0xF800) >> 11) * 527 + 23) >> 6; 
        g = (((color & 0x07E0) >> 5) * 259 + 33) >> 6; 
        b = ((color & 0x001F) * 527 + 23) >> 6;
        }
	

	PREFIX int16_t SSD_Util::sizeCheck(int16_t origin,int16_t len,int16_t maxVal)
	{
		if (((origin + len) - 1) >= maxVal) len = maxVal - origin;
		return len;
	}

	PREFIX void SSD_Util::_convertColor(uint16_t color,uint8_t &r,uint8_t &g,uint8_t &b)
	{
		r = (uint8_t)((color >> 11) << 1);
		g =	(uint8_t)((color >> 5) & 0x3F);
		b = (uint8_t)((color << 1) & 0x3F);
	}
		
/**************************************************************************/
/*!
	  calculate a gradient color
	  return a spectrum starting from blue to red (0...127)
	  From my RA8875 library
*/
/**************************************************************************/
//+++++++++OK
PREFIX uint16_t SSD_Util::gradient(uint8_t val)
{
	uint8_t r = 0;
	uint8_t g = 0;
	uint8_t b = 0;
	uint8_t q = val / 32;
	switch(q){
		case 0: r = 0; g = 2 * (val % 32); b = 31; break;
		case 1: r = 0; g = 63; b = 31 - (val % 32); break;
		case 2: r = val % 32; g = 63; b = 0; break;
		case 3: r = 31; g = 63 - 2 * (val % 32); b = 0; break;
	}
	return (r << 11) + (g << 5) + b;
}
/**************************************************************************/
/*!
	  interpolate 2 r,g,b colors
	  return a 16bit mixed color between the two
	  Parameters:
	  r1.
	  g1:
	  b1:
	  r2:
	  g2:
	  b2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
//+++++++++OK
PREFIX uint16_t SSD_Util::colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return Color565(r1,g1,b1);
    if (pos >= div) return Color565(r2,g2,b2);
	float pos2 = (float)pos/div;
	return Color565(
				(uint8_t)(((1.0 - pos2) * r1) + (pos2 * r2)),
				(uint8_t)(((1.0 - pos2) * g1) + (pos2 * g2)),
				(uint8_t)(((1.0 - pos2) * b1) + (pos2 * b2))
	);
}

/**************************************************************************/
/*!
	  interpolate 2 16bit colors
	  return a 16bit mixed color between the two
	  Parameters:
	  color1:
	  color2:
	  pos:0...div (mix percentage) (0:color1, div:color2)
	  div:divisions between color1 and color 2
	  From my RA8875 library
	  NOTE: Needs a remake, it's slow! (TODO)
*/
/**************************************************************************/
//+++++++++OK
PREFIX uint16_t SSD_Util::colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div)
{
    if (pos == 0) return color1;
    if (pos >= div) return color2;
	uint8_t r1,g1,b1;
	Color565ToRGB(color1,r1,g1,b1);//split in r,g,b
	uint8_t r2,g2,b2;
	Color565ToRGB(color2,r2,g2,b2);//split in r,g,b
	return colorInterpolation(r1,g1,b1,r2,g2,b2,pos,div);
}


/**************************************************************************/
/*!
		sin e cos helpers
		[private]
		from my RA8875 library
*/
/**************************************************************************/
PREFIX float SSD_Util::cosDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return cos(rads);
}


PREFIX float SSD_Util::sinDeg_helper(float angle)
{
	float rads = angle / (float)360 * 2 * PI;
	return sin(rads);
}

#undef PREFIX
