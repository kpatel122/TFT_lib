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
	void fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);
	void drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color);
	void drawPixel(int32_t x, int32_t y, uint32_t color);
	void drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color);
	void drawFastVLine(int32_t x, int32_t y, int32_t h, uint32_t color);
	void drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t r, uint32_t color);
	void fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, uint32_t color);


	void drawCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
	void drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, uint32_t color);
	void fillCircle(int32_t x, int32_t y, int32_t r, uint32_t color);
	void fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, uint32_t color);


	void init();


	void writecommand(uint8_t c);
	void writedata(uint8_t d);
	void setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1);
	void pushBlock(uint16_t color, uint32_t len);
	void initLcd();
	void setRotation(uint8_t m);
	void resetViewport();

	uint8_t rotation;  // Display rotation (0-3)

protected:
	  int32_t  _init_width, _init_height; // Display w/h as input, used by setRotation()
	  int32_t  _width, _height;           // Display w/h as modified by current rotation
	  int32_t  addr_row, addr_col;        // Window position - used to minimise window commands
	  bool     inTransaction; // SPI transaction and mutex lock flags

	  // Viewport variables
	  int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
	  int32_t  _xDatum;
	  int32_t  _yDatum;
	  int32_t  _xWidth;
	  int32_t  _yHeight;
	  bool     _vpDatum;
	  bool     _vpOoB;

  private:

	inline void tft_Write_8(uint8_t C)  {   GPIO.out_w1tc = clr_mask; GPIO.out_w1ts = set_mask((uint8_t)(C));WR_L; WR_H; } //WR_H

    // Bit masks for ESP32 parallel bus interface
   uint32_t xclr_mask, xdir_mask; // Port set/clear and direction control masks

              // Lookup table for ESP32 parallel bus interface uses 1kbyte RAM,
   uint32_t xset_mask[256]; // Makes Sprite rendering test 33% faster, for slower macro equivalent
                              // see commented out #define set_mask(C) within TFT_eSPI_ESP32.h




};
#endif

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */


