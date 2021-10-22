#ifndef SSD_1351_REGISTERS_H
#define SSD_1351_REGISTERS_H

#include <stdint.h>

// SSD_1351 sets data bit for command data.
#define SSD_CMD_DATA_BIT 1

// Row and column address commands.
static const uint8_t CMD_SETCOLUMN              = 0x15;
static const uint8_t CMD_SETROW   	            = 0x75;

// Read / write RAM commands.
static const uint8_t CMD_WRITERAM		        = 0x5C;
static const uint8_t CMD_READRAM		        = 0x5D;

// Set contrast commands.
static const uint8_t CMD_CONTRASTABC	        = 0xC1;



// Master current command. (Brightness)
static const uint8_t CMD_MASTERCURRENT          = 0xC7;






// Set remap register command.
static const uint8_t CMD_SETREMAP 		        = 0xA0;

// Bitmask for rotation bits in remap.
static const uint8_t VAL_REMAP_ROTATION_BITS    = 0x1b;

// Start and offset commands.
static const uint8_t CMD_STARTLINE 		        = 0xA1;
static const uint8_t CMD_DISPLAYOFFSET      	= 0xA2;

// Display mode commands.
static const uint8_t CMD_NORMALDISPLAY          = 0xA6;
static const uint8_t CMD_DISPLAYALLON 	        = 0xA5;
static const uint8_t CMD_DISPLAYALLOFF 	        = 0xA4;
static const uint8_t CMD_INVERTDISPLAY          = 0xA7;

// Set mux ratio command.
static const uint8_t CMD_SETMULTIPLEX 	        = 0xCA;




// Set master config command.
static const uint8_t CMD_SETCONFIG		        = 0xAB;




// Display on / off commands.
static const uint8_t CMD_DISPLAYOFF 	        = 0xAE;
static const uint8_t CMD_DISPLAYON  	        = 0xAF;






// Display enhance command.
static const uint8_t CMD_DISPLAYENHANCE	        = 0xB2;
static const uint8_t VAL_DISPLAYENHANCE_ON[]    = {0xA4, 0, 0};
static const uint8_t VAL_DISPLAYENHANCE_OFF[]   = {0, 0, 0};

// Command for phase period.
static const uint8_t CMD_PHASEPERIOD 	        = 0xB1;

// Command for clock div setting.
static const uint8_t CMD_CLOCKDIV 		        = 0xB3;

// Set VSL command and values.
static const uint8_t CMD_SETVSL			        = 0xB4;

// Set GPIO command.
static const uint8_t CMD_SETGPIO		        = 0xB5;

// Set second precharge period command and default value.
static const uint8_t CMD_PRECHARGEPERIOD		= 0xB6;

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
static const uint8_t VAL_LOCK_COMMANDS          = 0xB0;
static const uint8_t VAL_UNLOCK_COMMANDS        = 0xB1;









// Scroll commands.
static const uint8_t CMD_SCROLL_SETUP	        = 0x96;
static const uint8_t CMD_SCROLL_OFF 	        = 0x9E;
static const uint8_t CMD_SCROLL_ON  	        = 0x9F;

#endif
