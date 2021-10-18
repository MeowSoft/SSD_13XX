#ifndef _SSD_13XXLIB_H_
#define _SSD_13XXLIB_H_

// Hardware config. (Select display type here)
#include "Hardware/Config.h"

// SPI driver lib.
#include "Spi_Driver.h"

// Screen config class.
#include "SSD_ScreenConfig.h"

class SSD_Core {

    public:

        /**
         * @brief Constructor to set the SPI driver instance and reset pin.
         * 
         * @param spi 
         * @param rstPin 
         */
        SSD_Core(
            SPI_Driver spi, 
            const uint8_t rstPin
        );

        // ===================================================
        /* #region Screen configuration methods:            */
        // ===================================================

        /**
         * @brief Initialize screen hardware.
         */
	    void init(void);

        /**
         * @brief Reset screen hardware.
         */
        void resetScreen(void);

        /**
         * @brief Set screen mode.
         * 
         * @param mode The mode to set. See 'ScreenModes' in the 
         * 'SSD_ScreenConfig' class for descriptions of modes to set.
         */
	    void setScreenMode(const enum SSD_ScreenConfig::ScreenModes mode);

        /**
         * @brief Get current screen mode.
         * 
         * @return SSD_ScreenConfig::ScreenModes 
         */
	    SSD_ScreenConfig::ScreenModes getScreenMode(void);

        /**
         * @brief Set the screen rotation.
         * 
         * @param rotation The rotation to set.
         */
	    void setScreenRotation(SSD_ScreenConfig::Rotations rotation);

        /**
         * @brief Get the screen rotation.
         * 
         * @return SSD_ScreenConfig::Rotations 
         */
	    SSD_ScreenConfig::Rotations getScreenRotation(void);

        /**
         * @brief Get screen height.
         * 
         * @return int16_t 
         */
	    int16_t	getScreenHeight(void) const;

        /**
         * @brief Get screen width.
         * 
         * @return int16_t 
         */
	    int16_t getScreenWidth(void) const;

        /**
         * @brief Set the screen color depth.
         * 
         * @param depth Can be 8, 16, or (SSD_1351 only) 18. 
         * Other values will be coerced to one of these.
         */
	    void setScreenColorDepth(uint8_t depth);

        /**
         * @brief Set the screen color order.
         * 
         * @param order Color order to use.
         */
	    void setScreenColorOrder(SSD_Hardware::ColorOrder_t order);

        /**
         * @brief Set the address window to write screen data to.
         * 
         * @param x0 
         * @param y0 
         * @param x1 
         * @param y1 
         */
	    void setAddressWindow(
            int16_t x0, 
            int16_t y0, 
            int16_t x1, 
            int16_t y1
        );

        /**
         * @brief Set screen brightness level.
         * 
         * @param brightness Level to set. 0 - 15. 
         * Values higher than 15 will be clamped.
         */
	    void setScreenBrightness(uint8_t brightness);

        /* #endregion */
        // ===================================================

        // ===================================================
        /* #region Scroll methods:                          */
        // ===================================================

        /**
         * @brief Set up an area of the screen to scroll.
         * 
         * @param a Number of columns to horizontal scroll by per scroll step. (1331) OR 
         *          direction of horizontal scroll (1351)
         *              0x3F or less = scroll towards SEG127, 
         *              greater than 0x3F = scroll towards SEG0 
         * 
         * @param b Address of first row in scroll area.
         * @param c Number of rows in scroll area.
         * @param d Number of rows to vertical scroll by per scroll step. (1331 only)
         * @param e Number of frames per scroll step.
         */
        void defineScrollArea(
            uint8_t a, 
            uint8_t b, 
            uint8_t c, 
            uint8_t d, 
            uint8_t e
        );

        /**
         * @brief Turn scroll on or off.
         * 
         * @param active 
         */
	    void scroll(bool active);

        /* #endregion */
        // ===================================================

        // ===================================================
        /* #region Area methods:                            */
        // ===================================================

        /**
         * @brief Copy an area of the screen.
         * 
         * @param sourceX 
         * @param sourceY 
         * @param width 
         * @param height 
         * @param destinationX 
         * @param destinationY 
         */
        void copyArea(
            int16_t sourceX, 
            int16_t sourceY, 
            int16_t width, 
            int16_t height,
            int16_t destinationX, 
            int16_t destinationY
        );

        /**
         * @brief Dim an area of the screen.
         * 
         * @param x 
         * @param y 
         * @param width 
         * @param height 
         */
        void dimArea(
            int16_t x, 
            int16_t y, 
            int16_t width, 
            int16_t height
        );

        /**
         * @brief Clear an area of the screen.
         * 
         * @param x 
         * @param y 
         * @param width 
         * @param height 
         */
        void clearArea(
            int16_t x, 
            int16_t y, 
            int16_t width, 
            int16_t height
        );

        /* #endregion */
        // ===================================================

        // ===================================================
        /* #region Drawing methods:                         */
        // ===================================================

        /**
         * @brief Fill the screen with the given color.
         * 
         * @param color 
         */
        void fillScreen(uint16_t color);	

        /**
         * @brief Draw a single pixel.
         * 
         * @param x 
         * @param y 
         * @param color 
         */
	    void drawPixel(
            int16_t x, 
            int16_t y, 
            uint16_t color
        );

        /**
         * @brief Draw a line.
         * 
         * @param x0 
         * @param y0 
         * @param x1 
         * @param y1 
         * @param color 
         */
	    void drawLine(
            int16_t x0, 
            int16_t y0,
            int16_t x1, 
            int16_t y1, 
            uint16_t color
        );

        /**
         * @brief Draw a rectangle.
         * 
         * @param x Starting x coordinate.
         * @param y Starting Y coordinate.
         * @param width Rectangle width.
         * @param height Rectangle height.
         * @param color1 Rectangle outline color.
         * @param color2 Rectangle fill color.
         * @param filled Fill rectangle or only outline.
         */
	    void drawRectangle(
            int16_t x, 
            int16_t y, 
            int16_t width, 
            int16_t height, 
            uint16_t color1,
            uint16_t color2,
            bool filled
        );

        /**
         * @brief Draw a rectangle shaped color gradient.
         * 
         * @param x Starting x coordinate.
         * @param y Starting y coordinate.
         * @param width Gradient width.
         * @param height Gradient height.
         * @param color1 Gradient starting color.
         * @param color2 Gradient ending color.
         */
        void drawGradient(
            int16_t x, 
            int16_t y, 
            int16_t width, 
            int16_t height, 
            uint16_t color1,
            uint16_t color2
        );

        /* #endregion */
        // ===================================================

    private:
    
        // Screen reset pin.
        uint8_t _rstPin;

        // SPI driver.
        SPI_Driver _spi;

        // Screen config settings.
        SSD_ScreenConfig _screenConfig;

        // =======================================================
        /* #region Inline methods defined in SSD_Core.ipp:      */
        // =======================================================

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
         */
        bool _checkBounds(
            int16_t x,
            int16_t y
        );

        /**
         * @brief Coerce the given x and y coordinates to screen bounds.
         * 
         * @param x 
         * @param y 
         */
        void _coerceBounds(
            int16_t& x,
            int16_t& y
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
        void _drawGradient(
            int16_t x, 
            int16_t y, 
            int16_t w, 
            int16_t h, 
            uint16_t color1,
            uint16_t color2
        );
        
        /* #endregion */
        // =======================================================

        // =======================================================
        /* #region Private methods defined in SSD_Core.cpp:     */
        // =======================================================

        void _setPreCharge();
        void _setDimModeContrast();
        void _setEnhanceDisplay(bool enhance);

        /**
         * @brief Init code for 133x hardware.
         */
        void _init133x();

        /**
         * @brief Init code for 1351 hardware.
         */
        void _init1351();

        /**
         * @brief Issue command to set Screen contrast.
         */
        void _setContrast();

        /**
         * @brief Issue command to set screen color levels.
         */
        void _setColorLevels();

        /**
         * @brief Issue command to load screen greyscale table.
         */
        void _setGreyscaleTable();

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

        // SSD_1331 and SSD_1332 only:
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

        /* #endregion */
        // =======================================================

        // Give text and graphics engines access to private methods here.
        friend class Text_Engine;
        friend class Graphics_Engine;
        friend class SSD_ScreenConfig;
};

// Inline method definitions.
#include "SSD_Core.ipp"

#endif
