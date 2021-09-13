#ifndef SSD_SCREEN_CONFIG_H
#define SSD_SCREEN_CONFIG_H

#include <stdint.h>
#include "_settings/SSD_13XX_settings.h"
#include "SSD_13XX.h"
#include "SSD_Util.h"

class SSD_ScreenConfig {

    public:

        void init() {

            _remapReg = 0;
            _portrait = false;
            #if defined(_SSD_1331_96X64_H) || defined(_SSD_1332_96X64_H)
            if (SSD_COMSPLIT == 1){
                _remapReg |= ((1 << 5));
            } else {
                _remapReg |= ((0 << 5));
            }
            #endif
        };

        void setColorDepth(uint8_t depth)
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
        };

        void setColorOrder(bool order)
        {
        
            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
                _remapReg |= ((order << 2));
            #endif
        };

        void setRotation(uint8_t m) {

            _width  = SSD_WIDTH;
            _height = SSD_HEIGHT;

            _rotation = m % 4; // can't be higher than 3
	_portrait = false;
	_remapReg &= ~(0x1B);//clear bit 0,1,3,4
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
        };

int16_t getWidth() const {
    return _width;
};
int16_t getHeight() const {
    return _height;
};
bool isPortrait() {return _portrait; }

uint8_t getRotation() {
    return _rotation;
};
uint8_t getRemap() {
    return _remapReg;
}

    private:

        volatile uint8_t _remapReg;
	volatile int16_t		_width, _height;
	uint8_t					_colorDepth;
	uint8_t					_rotation;
	bool				_portrait;

};

#endif
