#ifndef SSD_1332_REGISTERS_H
#define SSD_1332_REGISTERS_H

#include <stdint.h>

// SSD_1332 does not set data bit for command data.
#undef SSD_CMD_DATA_BIT 

// Row and column address commands.
static const uint8_t CMD_SETCOLUMN              = 0x15;
static const uint8_t CMD_SETROW   	            = 0x75;





// Set contrast commands.
static const uint8_t CMD_CONTRASTA 	            = 0x81;
static const uint8_t CMD_CONTRASTB 	            = 0x82;
static const uint8_t CMD_CONTRASTC 	            = 0x83;

// Master current command. (Brightness)
static const uint8_t CMD_MASTERCURRENT          = 0x87;






// Set remap register command.
static const uint8_t CMD_SETREMAP 		        = 0xA0;

// Bitmask for rotation bits in remap.
static const uint8_t VAL_REMAP_ROTATION_BITS    = 0x1b;

// Start and offset commands.
static const uint8_t CMD_STARTLINE 		        = 0xA1;
static const uint8_t CMD_DISPLAYOFFSET 	        = 0xA2;

// Display mode commands.
static const uint8_t CMD_NORMALDISPLAY          = 0xA4;
static const uint8_t CMD_DISPLAYALLON 	        = 0xA5;
static const uint8_t CMD_DISPLAYALLOFF 	        = 0xA6;
static const uint8_t CMD_INVERTDISPLAY          = 0xA7;

// Set mux ratio command.
static const uint8_t CMD_SETMULTIPLEX 	        = 0xA8;




// Set master config command.
static const uint8_t CMD_SETCONFIG		        = 0xAD;




// Display on / off commands.
static const uint8_t CMD_DISPLAYOFF 	        = 0xAE;
static const uint8_t CMD_DISPLAYON  	        = 0xAF;

// Power save mode command.
static const uint8_t CMD_POWERMODE 	            = 0xB0;
static const uint8_t VAL_POWERMODE_ON           = 0x12;
static const uint8_t VAL_POWERMODE_OFF          = 0x00;






// Command for phase period.
static const uint8_t CMD_PHASEPERIOD 	        = 0xB1;

// Command for clock div setting.
static const uint8_t CMD_CLOCKDIV 		        = 0xB3;










// Command to load greyscale table data or use a linear table.
static const uint8_t CMD_GRAYSCALE 		        = 0xB8;
static const uint8_t CMD_LINEARGRAY 	        = 0xB9;

// Set precharge voltage commands.
static const uint8_t CMD_PRECHARGELEVELA 	    = 0xBB;
static const uint8_t CMD_PRECHARGELEVELB 	    = 0xBC;
static const uint8_t CMD_PRECHARGELEVELC 	    = 0xBD;

// Set com deselect level command.
static const uint8_t CMD_VCOMH 			        = 0xBE;

// NOP command.
static const uint8_t CMD_NOP     		        = 0xE3;






// Drawing commands.
static const uint8_t CMD_DRAWLINE 	            = 0x21;
static const uint8_t CMD_DRAWRECT 	            = 0x22;
static const uint8_t CMD_COPYWINDOW             = 0x23;
static const uint8_t CMD_DIMWINDOW 	            = 0x24;
static const uint8_t CMD_CLRWINDOW 	            = 0x25;
static const uint8_t CMD_FILLMODE	            = 0x26;






#endif
