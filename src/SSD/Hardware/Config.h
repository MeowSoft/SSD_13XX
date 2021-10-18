#ifndef CONFIG_H
#define CONFIG_H

// Select display hardware type here.

// #include "Displays/SSD_1331_96x64.h"
#include "Displays/Adafruit_684_96x64.h"
//#include "_display/SSD_1331_REDPCB.h"
//#include "_display/SSD_1332_96x64.h"
//#include "_display/SSD_1351_128x128.h"

// Display data class.
namespace SSD_Hardware {
static SSD_DisplayData displayData = SSD_DisplayData();
}

#include "Displays/SSD_DisplayDataMacros.h"

#endif
