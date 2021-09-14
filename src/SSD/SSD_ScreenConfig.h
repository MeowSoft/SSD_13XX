#ifndef SSD_SCREEN_CONFIG_H
#define SSD_SCREEN_CONFIG_H

#include <stdint.h>
#include "_settings/SSD_13XX_settings.h"
// #include "SSD_13XX.h"
#include "SSD_Util.h"

class SSD_13XX;



class SSD_ScreenConfig {

    public:

        enum Rotations {
            ROTATION_LANDSCAPE = 0, 
            ROTATION_PORTRAIT, 
            ROTATION_LANDSCAPE_REVERSED, 
            ROTATION_PORTRAIT_REVERSED 
        };
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

        enum  ScreenModes {
            NORMAL = 0,
            PWRSAVE,
            INVERT,
            DISP_ON, 
            DISP_DIM,
            DISP_OFF,
            PROTECT,
            ALL_ON,
            ALL_OFF
        };

        /**
         * @brief Initialize the screen configuration.
         * 
         * @param ssd Reference to the containing SSD object. 
         * This is needed for access to SPI methods to write 
         * the remap register and change modes. 
         */
        void init(SSD_13XX* ssd);

        /**
         * @brief Set the screen color depth.
         * 
         * @param depth Can be 8, 16, or (SSD_1351 only) 18. 
         * Other values will be coerced to one of these.
         */
        void setColorDepth(uint8_t depth);

        /**
         * @brief Set the screen color order.
         * 
         * @param useBGR Set TRUE to use BGR instead of RGB.
         */
        void setColorOrder(bool useBGR);

        /**
         * @brief Set screen rotation.
         * 
         * @param rotation Rotation value.
         */
        void setRotation(Rotations rotation);

        /**
         * @brief Write the remap register. This method must be
         * called after calling 'init', 'setColorDepth' setColorOrder' 
         * or 'setRotation' to commit the changes to the display.
         */
        void writeRemap();

        /**
         * @brief Set the screen mode.
         * 
         * @param mode Mode to set. See the enum 
         * definition for a desription of each mode.
         */
        void changeMode(ScreenModes mode);

        // Getters:
        uint8_t getMode() { return _currentMode; };
        int16_t getWidth() const { return _width; };
        int16_t getHeight() const { return _height; };
        bool isPortrait() { return _portrait; }
        uint8_t getRotation() { return _rotation; };
        uint8_t getRemap() { return _remapReg; }

    private:

        SSD_13XX* _ssd;

        volatile uint8_t _remapReg;
        volatile int16_t _width;
        volatile int16_t _height;
        uint8_t	_colorDepth;
        uint8_t	_rotation;
        bool _portrait;
	    uint8_t	_currentMode;
};

#endif
