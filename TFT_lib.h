#ifndef TFT_LIB_H
#define TFT_LIB_H

#define TFT_WIDTH  320
#define TFT_HEIGHT 480

#include <arduino.h>

#include <TFT_setup.h>

#if defined (ESP32)
#include <pgmspace.h>
  #include "Processors/TFT_esp32.h"
#endif

class TFT_Screen {

 public:

	TFT_Screen(int16_t _W = TFT_WIDTH, int16_t _H = TFT_HEIGHT);

	  // Graphics drawing
	void fillScreen(uint32_t color);
	void fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color);

	void init();


	void writecommand(uint8_t c);
	void writedata(uint8_t d);
	void setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
	void pushBlock(uint16_t color, uint32_t len);
	void init_lcd();

  private:
    byte pin;


    // Bit masks for ESP32 parallel bus interface
   uint32_t xclr_mask, xdir_mask; // Port set/clear and direction control masks

              // Lookup table for ESP32 parallel bus interface uses 1kbyte RAM,
   uint32_t xset_mask[256]; // Makes Sprite rendering test 33% faster, for slower macro equivalent
                              // see commented out #define set_mask(C) within TFT_eSPI_ESP32.h


   int16_t _width, _height;
   int32_t  addr_row, addr_col;        // Window position - used to minimise window commands


};
#endif


