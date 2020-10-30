#ifndef SSD_1351_REGISTERS_H
#define SSD_1351_REGISTERS_H

static const uint8_t CMD_SETCOLUMN      = 0x15;
static const uint8_t CMD_SETROW   	    = 0x75;
static const uint8_t CMD_WRITERAM		= 0x5C;
static const uint8_t CMD_READRAM		= 0x5D;
static const uint8_t CMD_SETREMAP 		= 0xA0;
static const uint8_t CMD_STARTLINE 		= 0xA1;
static const uint8_t CMD_DISPLAYOFFSET 	= 0xA2;
static const uint8_t CMD_DISPLAYALLOFF 	= 0xA4;
static const uint8_t CMD_DISPLAYALLON 	= 0xA5;
static const uint8_t CMD_NORMALDISPLAY  = 0xA6;
static const uint8_t CMD_INVERTDISPLAY  = 0xA7;
static const uint8_t CMD_SETCONFIG		= 0xAB;
static const uint8_t CMD_DISPLAYOFF 	= 0xAE;
static const uint8_t CMD_DISPLAYON  	= 0xAF;
static const uint8_t CMD_PHASEPERIOD 	= 0xB1;
static const uint8_t CMD_DISPLAYENHANCE	= 0xB2;
static const uint8_t CMD_CLOCKDIV 		= 0xB3;
static const uint8_t CMD_SETVSL			= 0xB4;
static const uint8_t CMD_SETGPIO		= 0xB5;
static const uint8_t CMD_PRECHARGE2		= 0xB6;
static const uint8_t CMD_GRAYSCALE 		= 0xB8;
static const uint8_t CMD_LINEARGRAY 	= 0xB9;
static const uint8_t CMD_PRECHARGELEVEL = 0xBB;
static const uint8_t CMD_VCOMH 			= 0xBE;
static const uint8_t CMD_CONTRASTABC	= 0xC1;
static const uint8_t CMD_MASTERCURRENT  = 0xC7;
static const uint8_t CMD_SETMULTIPLEX 	= 0xCA;
static const uint8_t CMD_NOP     		= 0xE3;
static const uint8_t CMD_CMDLOCK	 	= 0xFD;
static const uint8_t CMD_HORIZSCROLL	= 0x96;
static const uint8_t CMD_SCROLL_OFF 	= 0x9E;
static const uint8_t CMD_SCROLL_ON  	= 0x9F;

static const uint8_t VAL_CMDLOCK_UNLOCK = 0x12;
static const uint8_t VAL_CMDLOCK_LOCK   = 0x16;

static const uint8_t VAL_REMAP_ROTATION_BITS = 0x1b;

#endif
