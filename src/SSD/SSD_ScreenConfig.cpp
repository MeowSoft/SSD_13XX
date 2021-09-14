#include "SSD_ScreenConfig.h"
#include "SSD_13XX.h"

 void SSD_ScreenConfig::init(SSD_13XX* ssd) {

_ssd = ssd;
	_currentMode	= 0b00000000;

            _remapReg = 0;
            _portrait = false;
            #if defined(_SSD_1331_96X64_H) || defined(_SSD_1332_96X64_H)
            if (SSD_COMSPLIT == 1){
                _remapReg |= ((1 << 5));
            } else {
                _remapReg |= ((0 << 5));
            }
            #endif
        }

void SSD_ScreenConfig::setColorDepth(uint8_t depth)
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
        }

        void SSD_ScreenConfig::setColorOrder(bool useBGR)
        {
        
            #if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
                _remapReg |= ((useBGR << 2));
            #endif
        }

void SSD_ScreenConfig::changeMode(ScreenModes mode) {

    // If mode is already set, then bail.
	if (mode == _currentMode) return;

	_ssd->_spi.startTransaction();

		switch(mode){

			case NORMAL:

				if (_currentMode == 6) {//was in off display? PROTECT
					_ssd->_spi.writeCommand8(CMD_DISPLAYON);
				}
				/*
				if (_currentMode == 2) {//was in idle?
					//_spi.writeCommand8(CMD_IDLEOF);
				}
				*/
				
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
				if (_currentMode == 8) {//was in powerMode?
					_ssd->_writeRegister(CMD_POWERMODE,SSD_POWERMODE);
					//delay(120);//needed
				}
				#endif
				
				if (_currentMode == 4){//was inverted?
					//SSD1332 should need only CMD_NORMALDISPLAY!?!
					#if defined(SSD_1331_REGISTERS_H)
					//_spi.writeCommand8(CMD_DINVOF);
					#endif
				}
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
				if (_currentMode == 9){//was in protect mode?
					_ssd->_writeRegister(CMD_CMDLOCK,0x12);//unlock
				}
				#endif
				/*
				if (_currentMode == 12 || _currentMode == 13){//all on or off?
					//nothing
				}
				*/
				_ssd->_spi.writeCommand8(CMD_NORMALDISPLAY);
				_currentMode = 0;
			break;
			case ALL_ON:
				_ssd->_spi.writeCommand8(CMD_DISPLAYALLON);
				_currentMode = 12;
			break;
			case ALL_OFF:
				_ssd->_spi.writeCommand8(CMD_DISPLAYALLOFF);
				_currentMode = 13;
			break;
			case PWRSAVE: //power mode ON
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1332_REGISTERS_H)
					_ssd->_spi.writeCommand8(CMD_POWERMODE);
					_ssd->_spi.writeCommand8AndDeselect(0x1A);
					_currentMode = 8;
					delay(5);//needed
				#else
					//TODO: exist?
					//_spi.writeData8AndDeselect(0x1A);
				#endif
				_ssd->_spi.endTransaction();
				return;
			case INVERT:
				_ssd->_spi.writeCommand8(CMD_INVERTDISPLAY);//OK
				_currentMode = 4;
			break;
			case DISP_ON:
				_ssd->_spi.writeCommand8(CMD_DISPLAYON);
				_currentMode = 5;
			break;
			case DISP_OFF:
				_ssd->_spi.writeCommand8(CMD_DISPLAYOFF);
				_currentMode = 6;
			break;
			/*
			case DISP_DIM:
				ssd->_spi.writeCommand8(CMD_DISPLAYDIM);
				_currentMode = 7;
			break;
			*/
			case PROTECT:
				#if defined(SSD_1331_REGISTERS_H) || defined(SSD_1351_REGISTERS_H)
				_ssd->_writeRegister(CMD_CMDLOCK,0x16);//lock
				_currentMode = 9;
				#else
					ssd->_spi.endTransaction();
					return;
				#endif
			break;
			default:
				_ssd->_spi.endTransaction();
				return;
			break;
		}
		_ssd->_spi.deselectAndEndTransaction();
	
}

void SSD_ScreenConfig::setRotation(Rotations rotation) {

            _width  = SSD_WIDTH;
            _height = SSD_HEIGHT;

            _rotation = rotation % 4; // can't be higher than 3
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
}
    void SSD_ScreenConfig::writeRemap() {

    _ssd->_spi.startTransaction();
		_ssd->_setAddressWindow(0,0,getHeight()-1,getWidth()-1,false);
		_ssd->_spi.writeCommand8(CMD_SETREMAP);//set remap
		#if defined(_SSD_USECMDASDATA)
			_ssd->_spi.writeCommand8AndDeselect(getRemap());
		#else
			_ssd->_spi.writeData8AndDeselect(getRemap());
		#endif
	_ssd->_spi.deselectAndEndTransaction();

    }



