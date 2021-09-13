/*
	SSD_13XX - A fast SPI driver for several Solomon Systech SSD13xx series driver
	Version: 1.0r3, beta release

	https://github.com/sumotoy/SSD_13XX

	The library has a settings file inside _settings/SSD_13XX_settings.h, open it and choose
	your display type. Display settings files are inside _display folder, you may need duplicate and change
	name so you can tweak as your needs but normally this is not raccomanded since some parameter can
	literally destroy your display.

	-------------------------------------------------------------------------------
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
	++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Special Thanks:
	Thanks to Paul Stoffregen for his beautiful Teensy3 and FIFO SPI.
	Thanks to Jnmattern & Marek Buriak for drawArc!, reaper7 for a hint and many others
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	Version:
	1.0r1: First version
	+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
	BugList of the current version:
		Scroll should work but it has to be recoded, it's on bottom of my TODO list
	Please report any!
	Benchmarks -----------------------------------------------------------------
	Screen fill              409	809		807		809		808		507		406
	Text                     3983	3907	3963	3891	2357	2378	2270
	Text2                    9564	9368	9517	9332	5047	5057	4910
	Lines                    5576	10976	10978	10974	5561	299		277
	Horiz/Vert Lines         6827	1800	1978	1964	1956	1954	1956
	Rectangles (outline)     4508	8916	8919	8915	1126	1376	1123
	Rectangles (filled)      4509	8924	8922	8920	1223	1499	1214
	Circles (filled)         3635	18787	4488	4482	4481	4479	4481
	Circles (outline)        4697	2851	2852	2849	2848	2847	2847
	Triangles (outline)      517	505		508		504		503		204		183
	Triangles (filled)       1916	11087	4284	4274	4276	4274	4275
	Rounded rects (outline)  1206	2942	1619	1586	1585	1587	1588
	Rounded rects (filled)   10271	8184	5814	5807	5036	5241	5032
	Icon Render              										1404	1312
	----------------------------------------------------------------------------
	Versions:            	 1st	hrdw	r11		remap	adly	ldly
Screen fill              11952
Text                     1988
Text2                    11004
Lines                    12550
Horiz/Vert Lines         5141
Arc                      178365
Rectangles (outline)     4108
Rectangles (filled)      91168
Circles (filled)         21413
Circles (outline)        21863
Triangles (outline)      4023
Triangles (filled)       30205
Rounded rects (outline)  7010
Rounded rects (filled)   99127
Icon Render              1754
*/

#ifndef _SSD_13XXLIB_H_
#define _SSD_13XXLIB_H_


#include "Arduino.h"

#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
#include <stdio.h>
#include <stdlib.h>
// #include "Print.h"
#include <SPI.h>
#include "Spi_Driver.h"

#include "_includes/_cpuCommons.h"
#include "_includes/_common_16bit_colors.h"
#include "_settings/SSD_13XX_settings.h"

//Load sumotoy universal descriptors (used in many other libraries)
// #include "_includes/sumotoy_fontDescription.h"
#include "_includes/sumotoy_imageDescription.h"
#include "_includes/sumotoy_iconDescription.h"

#include "SSD_ScreenConfig.h"


enum SSD_13XX_modes{NORMAL=0,PWRSAVE,INVERT,DISP_ON, DISP_DIM,DISP_OFF,PROTECT,ALL_ON,ALL_OFF};
enum SSD_13XX_iconMods{NONE=0,TRANSPARENT,REPLACE,BOTH};

class SSD_13XX {

 public:

    SSD_13XX(SPI_Driver spi, const uint8_t rstPin);


	void     	begin();
	void		setAddressWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void		changeMode(const enum SSD_13XX_modes m);
	uint8_t 	getMode(void);
	int16_t		height(void) const;
	int16_t 	width(void) const;
	int16_t		cgHeight(void) const;
	int16_t 	cgWidth(void) const;
	void		setRotation(uint8_t r);
	uint8_t 	getRotation(void);
	void 		setBackground(uint16_t color);
	void 		setForeground(uint16_t color);
	uint16_t 	getBackground(void);
	uint16_t 	getForeground(void);
	void 		setBrightness(uint8_t brightness);
	void 		setColorDepth(uint8_t depth);
	void 		setColorOrder(bool order);
	//---------------------------- GEOMETRY ------------------------------------------------
	void		fillScreen(uint16_t color);																														//OK
	void		fillScreen(uint16_t color1,uint16_t color2);																									//Not ROT
	void		clearScreen(void);//fill with color choosed in setBackground
	void 		copyArea(int16_t sx0, int16_t sy0, int16_t sx1, int16_t sy1,int16_t dx, int16_t dy);
	void 		dimArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void 		moveArea(int16_t x0, int16_t y0, int16_t x1, int16_t y1);
	void		drawPixel(int16_t x, int16_t y, uint16_t color);
	void		drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color);
	void		drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
	void		drawLine(int16_t x0, int16_t y0,int16_t x1, int16_t y1, uint16_t color);
	void		drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
	void		drawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2,bool filled);
	void		fillRect(int16_t x, int16_t y, int16_t w, int16_t h,uint16_t color);
	void		fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color1,uint16_t color2);


	//------------------------------- SCROLL ----------------------------------------------------
	void 		defineScrollArea(int16_t a, int16_t b, int16_t c, int16_t d, uint8_t e);
	boolean		scroll(bool active);


 protected:
	uint8_t 				_rstPin;





 private:
 
    SPI_Driver _spi;


	uint8_t					_currentMode;


	uint16_t				_defaultBgColor;
	uint16_t				_defaultFgColor;


    SSD_ScreenConfig _screenConfig;

    // ================================ 
    #pragma region Inline methods defined in SSD_Core.ipp:
    // ================================

    /**
     * @brief Calculate delay in microseconds for an 
     * operation based on window size (w and h).
     * 
     * @param w 
     * @param h 
     * @param maxDly 
     * @return int 
     */
	int	_calculateDelay(
        int16_t w, 
        int16_t h,
        int maxDly
    );

    /**
     * @brief Check if the given x and y 
     * coordinates are within the screen bounds.
     * 
     * @param x 
     * @param y 
     * @return true 
     * @return false 
     */
	bool _checkBounds(
        int16_t x,
        int16_t y
    );

    /**
     * @brief Set row and column start and end addresses. This  
     * method does not start or end the SPI transaction frame.
     * 
     * @param rowStart 
     * @param columnStart 
     * @param rowEnd 
     * @param columnEnd 
     * @param checkRotation 
     */
	void _setAddressWindow(
        uint16_t rowStart, 
        uint16_t columnStart, 
        uint16_t rowEnd, 
        uint16_t columnEnd,
        bool checkRotation = true
    );

    /**
     * @brief Draw a single pixel on the screen. This method 
     * does not start or end the SPI transaction frame.
     * 
     * @param x 
     * @param y 
     * @param color 
     */
    void _drawPixel(
        int16_t x, 
        int16_t y, 
        uint16_t color
    );

    /**
     * @brief Draw a rectangle outlined or filled.
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param color1 
     * @param color2 
     * @param filled 
     */
	void _drawRectangle(
        int16_t x, 
        int16_t y, 
        int16_t w, 
        int16_t h, 
        uint16_t color1,
        uint16_t color2, 
        bool filled
    );

    /**
     * @brief Draw a rectangle that will be filled 
     * with a gradient between the two given colors.
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param h 
     * @param color1 
     * @param color2 
     */
	void _drawRectangleWithGradient(
        int16_t x, 
        int16_t y, 
        int16_t w, 
        int16_t h, 
        uint16_t color1,
        uint16_t color2
    );
    
    #pragma endregion
    // ================================

    // ================================
    #pragma region Private methods defined in SSD_Core.cpp:
    // ================================

    /**
     * @brief Write data to a register. This method
     * does not start or end the SPI transaction frame.
     * 
     * @param cmd 
     * @param data 
     */
	void _writeRegister(
        const uint8_t cmd,
        uint8_t data
    );

    /**
     * @brief Draw a horizontal line. This method
     * does not start or end the SPI transaction frame.
     * 
     * @param x 
     * @param y 
     * @param w 
     * @param color 
     */
	void _drawHorizontalLine(
        int16_t x, 
        int16_t y, 
        int16_t w, 
        uint16_t color
    );

    /**
     * @brief Draw a vertical line. This method
     * does not start or end the SPI transaction frame.
     * 
     * @param x 
     * @param y 
     * @param h 
     * @param color 
     */
	void _drawVerticalLine(
        int16_t x, 
        int16_t y, 
        int16_t h, 
        uint16_t color
    );

    /**
     * @brief Draw a line.  This method does not 
     * start or end the SPI transaction frame.
     * 
     * @param x0 
     * @param y0 
     * @param x1 
     * @param y1 
     * @param color 
     */
	void _drawLine(
        int16_t x0, 
        int16_t y0, 
        int16_t x1, 
        int16_t y1, 
        uint16_t color
    );

	#if defined(SSD_1331_REGISTERS_H) || \
        defined(SSD_1332_REGISTERS_H)

    /**
     * @brief Enable or disable fill.
     * 
     * @param filling 
     */
	void _setFillState(bool filling);

    /**
     * @brief Write color data. This method does
     * not start or end the SPI transaction frame.
     * 
     * @param r 
     * @param g 
     * @param b 
     */
	void _writeColorData(uint8_t r,uint8_t g,uint8_t b);

	#endif

    #pragma endregion
    // ================================

    // Give text and graphics engines access to private methods here.
    friend class Text_Engine;
    friend class Graphics_Engine;
    friend class SSD_ScreenConfig;
};

// Inline method definitions.
#include "SSD_Core.ipp"

#endif