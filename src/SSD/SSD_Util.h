#ifndef SSD_UTIL_H
#define SSD_UTIL_H

#include <stdint.h>

/**
 * @brief Macro to swap two vars.
 */
#if !defined(swapVals)
	#if defined(ESP8266)
		#define swapVals(a, b) { int16_t t = a; a = b; b = t; }
	#else
		#define swapVals(a, b) { typeof(a) t = a; a = b; b = t; }
	#endif
#endif

/**
 * @brief A class to hold some utility methods.
 */
class SSD_Util {

    public:

		/**
		 * @brief Map individual RGB values to a 565 value.
		 * 
		 * @param r 
		 * @param g 
		 * @param b 
		 * @return uint16_t 
		 */
		static uint16_t Color565(
			uint8_t r, 
			uint8_t g, 
			uint8_t b
		);

		/**
		 * @brief Map a 24 bit RGB value to a 565 value.
		 * 
		 * @param color_ 
		 * @return uint16_t 
		 */
		static uint16_t Color24To565(int32_t color_);

		/**
		 * @brief Map 
		 * 
		 * @param color_ 
		 * @return uint16_t 
		 */
		static uint16_t htmlTo565(int32_t color_);
		static void 	Color565ToRGB(uint16_t color, uint8_t &r, uint8_t &g, uint8_t &b);
		

		static int16_t sizeCheck(int16_t origin,int16_t len,int16_t maxVal);
		static void _convertColor(uint16_t color,uint8_t &r,uint8_t &g,uint8_t &b);
		static uint16_t gradient(uint8_t val);
		static uint16_t colorInterpolation(uint8_t r1,uint8_t g1,uint8_t b1,uint8_t r2,uint8_t g2,uint8_t b2,uint16_t pos,uint16_t div);
		static uint16_t colorInterpolation(uint16_t color1,uint16_t color2,uint16_t pos,uint16_t div);
		static float cosDeg_helper(float angle);
		static float sinDeg_helper(float angle);
};

#include "SSD_Util.ipp"

#endif
