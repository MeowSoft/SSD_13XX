#ifndef SSD_1332_REGISTERS_H
#define SSD_1332_REGISTERS_H

#define _SSD_USECMDASDATA 1

static const uint8_t CMD_SETCOLUMN      = 0x15;
static const uint8_t CMD_SETROW   	    = 0x75;
static const uint8_t CMD_CONTRASTA 	    = 0x81;
static const uint8_t CMD_CONTRASTB 	    = 0x82;
static const uint8_t CMD_CONTRASTC 	    = 0x83;
static const uint8_t CMD_MASTERCURRENT  = 0x87;
static const uint8_t CMD_SETREMAP 		= 0xA0;
static const uint8_t CMD_STARTLINE 		= 0xA1;
static const uint8_t CMD_DISPLAYOFFSET 	= 0xA2;
static const uint8_t CMD_NORMALDISPLAY  = 0xA4;
static const uint8_t CMD_DISPLAYALLON 	= 0xA5;
static const uint8_t CMD_DISPLAYALLOFF 	= 0xA6;
static const uint8_t CMD_INVERTDISPLAY  = 0xA7;
static const uint8_t CMD_SETMULTIPLEX 	= 0xA8;
static const uint8_t CMD_SETCONFIG		= 0xAD;
static const uint8_t CMD_DISPLAYOFF 	= 0xAE;
static const uint8_t CMD_DISPLAYON  	= 0xAF;
static const uint8_t CMD_POWERMODE 	    = 0xB0;
static const uint8_t CMD_PHASEPERIOD 	= 0xB1;
static const uint8_t CMD_CLOCKDIV 		= 0xB3;
static const uint8_t CMD_GRAYSCALE 		= 0xB8;
static const uint8_t CMD_LINEARGRAY 	= 0xB9;
static const uint8_t CMD_VPACOLORLVL 	= 0xBB;
static const uint8_t CMD_VPBCOLORLVL 	= 0xBC;
static const uint8_t CMD_VPCCOLORLVL 	= 0xBD;
static const uint8_t CMD_VCOMH 			= 0xBE;
static const uint8_t CMD_NOP     		= 0xE3;
static const uint8_t CMD_DRAWLINE 	    = 0x21;
static const uint8_t CMD_DRAWRECT 	    = 0x22;
static const uint8_t CMD_DRAWCOPY 	    = 0x23;
static const uint8_t CMD_DIMWINDOW 	    = 0x24;
static const uint8_t CMD_CLRWINDOW 	    = 0x25;
static const uint8_t CMD_FILL 		    = 0x26;

static const uint8_t VAL_POWERMODE_ON   = 0x12;
static const uint8_t VAL_POWERMODE_OFF  = 0x00;

static const uint8_t VAL_REMAP_ROTATION_BITS = 0x1b;

#endif
