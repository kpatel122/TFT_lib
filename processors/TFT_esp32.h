#ifndef TFT_ESP32_H
#define TFT_ESP32_H

#if defined (TFT_WR)
  #if (TFT_WR >= 0 && TFT_WR <= 31)
    // TFT_WR, by design, must be in range 0-31 for single register parallel write
    #define WR_L GPIO.out_w1tc = (1 << TFT_WR)
    #define WR_H GPIO.out_w1ts = (1 << TFT_WR)
  #else
	#error 'TFT_WR is greater than 31- please check setup.h'
  #endif
#else
#error 'No TFT_WR defined please check setup.h'

#endif


#define DC_C GPIO.out_w1tc = (1 << TFT_DC); GPIO.out_w1tc = (1 << TFT_DC)
#define DC_D GPIO.out_w1ts = (1 << TFT_DC); GPIO.out_w1ts = (1 << TFT_DC)

//#define DC_C RS_C
//#define DC_D RS_D

#define CS_L GPIO.out_w1tc = (1 << TFT_CS)
#define CS_H GPIO.out_w1ts = (1 << TFT_CS)


// Create a bit set lookup table for data bus - wastes 1kbyte of RAM but speeds things up dramatically
  // can then use e.g. GPIO.out_w1ts = set_mask(0xFF); to set data bus to 0xFF
#define CONSTRUCTOR_INIT_TFT_DATA_BUS            \
  for (int32_t c = 0; c<256; c++)                  \
  {                                                \
    xset_mask[c] = 0;                              \
    if ( c & 0x01 ) xset_mask[c] |= (1 << TFT_D0); \
    if ( c & 0x02 ) xset_mask[c] |= (1 << TFT_D1); \
    if ( c & 0x04 ) xset_mask[c] |= (1 << TFT_D2); \
    if ( c & 0x08 ) xset_mask[c] |= (1 << TFT_D3); \
    if ( c & 0x10 ) xset_mask[c] |= (1 << TFT_D4); \
    if ( c & 0x20 ) xset_mask[c] |= (1 << TFT_D5); \
    if ( c & 0x40 ) xset_mask[c] |= (1 << TFT_D6); \
    if ( c & 0x80 ) xset_mask[c] |= (1 << TFT_D7); \
  }                                                \

// Mask for the 8 data bits to set pin directions
#define dir_mask ((1 << TFT_D0) | (1 << TFT_D1) | (1 << TFT_D2) | (1 << TFT_D3) | (1 << TFT_D4) | (1 << TFT_D5) | (1 << TFT_D6) | (1 << TFT_D7))

// Data bits and the write line are cleared to 0 in one step
#define clr_mask (dir_mask) //| (1 << LCD_WR))

// A lookup table is used to set the different bit patterns, this uses 1kByte of RAM
#define set_mask(C) xset_mask[C] // 63fps Sprite rendering test 33% faster, graphicstest only 1.8% faster than shifting in real time

// Write 8 bits to TFT
#define tft_Write_8(C)  GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C));WR_L; WR_H;  //WR_H

#define tft_Write_16(C) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_L;WR_H; \
                           GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_L;WR_H


 // Write two concatenated 16 bit values to TFT

 // Write two concatenated 16 bit values to TFT
 #define tft_Write_32C(C,D) GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 8)); WR_L;WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((C) >> 0)); WR_L;WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((D) >> 8)); WR_L;WR_H; \
                             GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t) ((D) >> 0)); WR_L;WR_H

#endif //end TFT_ESP32_H
