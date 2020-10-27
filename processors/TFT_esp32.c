#include "Arduino.h"



void TFT_Screen::pushBlock(uint16_t color, uint32_t len)
{
  if ( (color >> 8) == (color & 0x00FF) )
  { if (!len) return;
    tft_Write_16(color);
  #if defined (SSD1963_DRIVER)
    while (--len) {WR_L; WR_H; WR_L; WR_H; WR_L; WR_H;}
  #else
    while (--len) {WR_L; WR_H; WR_L; WR_H;}
  #endif
  }
  else while (len--) {tft_Write_16(color);}
}



