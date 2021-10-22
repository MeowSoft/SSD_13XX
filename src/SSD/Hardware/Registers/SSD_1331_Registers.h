#ifndef SSD_1331_REGISTERS_H
#define SSD_1331_REGISTERS_H

#include <stdint.h>

// SSD_1331 does not set data bit for command data.
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

// Set pre charge commands and default values.
static const uint8_t CMD_PRECHARGESPEEDA 	    = 0x8A;
static const uint8_t CMD_PRECHARGESPEEDB		= 0x8B;
static const uint8_t CMD_PRECHARGESPEEDC 	    = 0x8C;

// Set remap register command.
static const uint8_t CMD_SETREMAP 		        = 0xA0;

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

// Set dim mode command.
static const uint8_t CMD_SETDIMLEVELS 	        = 0xAB;

// Set master config command.
static const uint8_t CMD_SETCONFIG		        = 0xAD;

// Dim mode command.
static const uint8_t CMD_DISPLAYDIM 	        = 0xAC;

// Display on / off commands.
static const uint8_t CMD_DISPLAYOFF 	        = 0xAE;
static const uint8_t CMD_DISPLAYON  	        = 0xAF;

// Power save mode command.
static const uint8_t CMD_POWERMODE 	            = 0xB0;
static const uint8_t VAL_POWERMODE_ON           = 0x1A;
static const uint8_t VAL_POWERMODE_OFF          = 0x0B;






// Command for phase period.
static const uint8_t CMD_PHASEPERIOD 	        = 0xB1;

// Command for clock div setting.
static const uint8_t CMD_CLOCKDIV 		        = 0xB3;










// Command to load greyscale table data or use a linear table.
static const uint8_t CMD_GRAYSCALE 		        = 0xB8;
static const uint8_t CMD_LINEARGRAY 	        = 0xB9;

// Set precharge voltage command.
static const uint8_t CMD_PRECHARGELEVEL         = 0xBB;



// Set com deselect level command.
static const uint8_t CMD_VCOMH 			        = 0xBE;

// NOP command.
static const uint8_t CMD_NOP     		        = 0xE3;

// Lock / unlock command.
static const uint8_t CMD_LOCK	 	            = 0xFD;
static const uint8_t VAL_UNLOCK                 = 0x12;
static const uint8_t VAL_LOCK                   = 0x16;

// Drawing commands.
static const uint8_t CMD_DRAWLINE 	            = 0x21;
static const uint8_t CMD_DRAWRECT 	            = 0x22;
static const uint8_t CMD_COPYWINDOW             = 0x23;
static const uint8_t CMD_DIMWINDOW 	            = 0x24;
static const uint8_t CMD_CLRWINDOW 	            = 0x25;
static const uint8_t CMD_FILLMODE	            = 0x26;

// Scroll commands.
static const uint8_t CMD_SCROLL_SETUP  	        = 0x27;
static const uint8_t CMD_SCROLL_OFF 	        = 0x2E;
static const uint8_t CMD_SCROLL_ON  	        = 0x2F;

#endif
