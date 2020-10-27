#ifndef TFT_SETUP_H
#define TFT_SETUP_H

#define TFT_RD   23
#define TFT_WR   22
#define TFT_DC   21  //RS pin on the screen module
#define TFT_CS   19
#define TFT_REST 18

#define TFT_RST TFT_REST


//data lines
#define TFT_D0 16 //RX2
#define TFT_D1 17 //TX2
#define TFT_D2 25
#define TFT_D3 26
#define TFT_D4 27
#define TFT_D5 14
#define TFT_D6 12
#define TFT_D7 13

#include "TFT_Drivers/ST7796_Defines.h"

#endif
