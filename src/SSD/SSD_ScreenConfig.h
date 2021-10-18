#ifndef SSD_SCREEN_CONFIG_H
#define SSD_SCREEN_CONFIG_H

#include <stdint.h>

// Forward ref for SSD pointer.
class SSD_Core;

class SSD_ScreenConfig {

    public:

        enum Rotations {
            ROTATION_LANDSCAPE = 0, 
            ROTATION_PORTRAIT, 
            ROTATION_LANDSCAPE_REVERSED, 
            ROTATION_PORTRAIT_REVERSED 
        };

        enum  ScreenModes {

            // Normal display.
            NORMAL = 0,

            // Power save mode. (Only on 1331 and 1332) 
            PWRSAVE,

            // Command lockout mode. (Only on 1331 and 1351)
            PROTECT,

            // Invert (negative) display mode.
            INVERT,

            // Display on. (Normal mode)
            DISP_ON, 

            // Display dim.
            DISP_DIM,

            // Display off. (Sleep mode)
            DISP_OFF,

            // Turn all pixels on. (Color will be last value in greyscale table)
            ALL_ON,

            // Turn all pixels off.
            ALL_OFF
        };

        /**
         * @brief Initialize the screen configuration.
         * 
         * @param ssd Reference to the containing SSD object. 
         * This is needed for access to SPI methods to write 
         * the remap register and change modes. 
         */
        void init(SSD_Core* ssd);

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
         * @param useBGR If true, useBGR instead of RGB.
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
        void writeRemap(void);

        /**
         * @brief Set the screen mode.
         * 
         * @param mode Mode to set. See the enum 
         * definition for a desription of each mode.
         */
        void changeMode(ScreenModes mode);

        // Getters:
        int16_t getWidth(void) const { return _width; };
        int16_t getHeight(void) const { return _height; };
        Rotations getRotation(void) { return _rotation; };
        ScreenModes getMode(void) { return _currentMode; };

        bool isPortrait(void) { 
            return (
                _rotation == ROTATION_PORTRAIT || 
                _rotation == ROTATION_PORTRAIT_REVERSED
            ); 
        }

    private:

        // Screen width and height. These values will swap based on screen rotation.
        volatile int16_t _width;
        volatile int16_t _height;

        // Screen rotation.
        Rotations _rotation;

        // Screen mode.
	    ScreenModes	_currentMode;

        // This reference allows us to do SPI stuff from this class.
        SSD_Core* _ssd;

        // Variable to hold the remap register contents.
        volatile uint8_t _remapReg;
};

#endif
