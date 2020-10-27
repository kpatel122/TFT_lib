#include "Arduino.h"
#include "TFT_lib.h"



#ifdef ESP32
#include "processors/TFT_esp32.c"
#endif

inline void begin_tft_write(void){CS_L;}
inline void end_tft_write(void){CS_H;}


void TFT_Screen::drawPixel(int32_t x, int32_t y, uint32_t color)
{
  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Range checking
  if ((x < _vpX) || (y < _vpY) ||(x >= _vpW) || (y >= _vpH)) return;

#ifdef CGRAM_OFFSET
  x+=colstart;
  y+=rowstart;
#endif

#if defined (SSD1963_DRIVER)
  if ((rotation & 0x1) == 0) { swap_coord(x, y); }
#endif

  begin_tft_write();

#ifdef MULTI_TFT_SUPPORT
  // No optimisation
  DC_C; tft_Write_8(TFT_CASET);
  DC_D; tft_Write_32D(x);
  DC_C; tft_Write_8(TFT_PASET);
  DC_D; tft_Write_32D(y);
#else
  // No need to send x if it has not changed (speeds things up)
  if (addr_col != (x<<16 | x)) {
    DC_C; tft_Write_8(TFT_CASET);
    DC_D; tft_Write_32D(x);
    addr_col = (x<<16 | x);
  }

  // No need to send y if it has not changed (speeds things up)
  if (addr_row != (y<<16 | y)) {
    DC_C; tft_Write_8(TFT_PASET);
    DC_D; tft_Write_32D(y);
    addr_row = (y<<16 | y);
  }
#endif

  DC_C; tft_Write_8(TFT_RAMWR);
  DC_D; tft_Write_16(color);

  end_tft_write();
}

void TFT_Screen::drawFastHLine(int32_t x, int32_t y, int32_t w, uint32_t color)
{

  if (_vpOoB) return;

  x+= _xDatum;
  y+= _yDatum;

  // Clipping
  if ((y < _vpY) || (x >= _vpW) || (y >= _vpH)) return;

  if (x < _vpX) { w += x - _vpX; x = _vpX; }

  if ((x + w) > _vpW) w = _vpW - x;

  if (w < 1) return;


  begin_tft_write();

  setWindow(x, y, x + w - 1, y);

  pushBlock(color, w);

  end_tft_write();
}

void TFT_Screen::fillEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) {
    drawFastHLine(x0 - x, y0 - y, x + x + 1, color);
    drawFastHLine(x0 - x, y0 + y, x + x + 1, color);

    if (s >= 0) {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

  inTransaction = false;
  end_tft_write();              // Does nothing if Sprite class uses this function
}

void TFT_Screen::drawEllipse(int16_t x0, int16_t y0, int32_t rx, int32_t ry, uint16_t color)
{
  if (rx<2) return;
  if (ry<2) return;
  int32_t x, y;
  int32_t rx2 = rx * rx;
  int32_t ry2 = ry * ry;
  int32_t fx2 = 4 * rx2;
  int32_t fy2 = 4 * ry2;
  int32_t s;

  //begin_tft_write();          // Sprite class can use this function, avoiding begin_tft_write()
  inTransaction = true;

  for (x = 0, y = ry, s = 2*ry2+rx2*(1-2*ry); ry2*x <= rx2*y; x++) {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0) {
      s += fx2 * (1 - y);
      y--;
    }
    s += ry2 * ((4 * x) + 6);
  }

  for (x = rx, y = 0, s = 2*rx2+ry2*(1-2*rx); rx2*y <= ry2*x; y++) {
    // These are ordered to minimise coordinate changes in x or y
    // drawPixel can then send fewer bounding box commands
    drawPixel(x0 + x, y0 + y, color);
    drawPixel(x0 - x, y0 + y, color);
    drawPixel(x0 - x, y0 - y, color);
    drawPixel(x0 + x, y0 - y, color);
    if (s >= 0)
    {
      s += fy2 * (1 - x);
      x--;
    }
    s += rx2 * ((4 * y) + 6);
  }

  inTransaction = false;
  end_tft_write();              // Does nothing if Sprite class uses this function
}

void TFT_Screen::setRotation(uint8_t m)
{
	begin_tft_write();

	  rotation = m % 8; // Limit the range of values to 0-7

	  writecommand(TFT_MADCTL);
	  switch (rotation) {
	    case 0:
	      writedata(TFT_MAD_MX | TFT_MAD_COLOR_ORDER);
	      _width  = _init_width;
	      _height = _init_height;
	      break;
	    case 1:
	      writedata(TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
	      _width  = _init_height;
	      _height = _init_width;
	      break;
	    case 2:
	      writedata(TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
	      _width  = _init_width;
	      _height = _init_height;
	      break;
	    case 3:
	      writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
	      _width  = _init_height;
	      _height = _init_width;
	      break;
	  // These next rotations are for bottom up BMP drawing
	    case 4:
	      writedata(TFT_MAD_MX | TFT_MAD_MY | TFT_MAD_COLOR_ORDER);
	      _width  = _init_width;
	      _height = _init_height;
	      break;
	    case 5:
	      writedata(TFT_MAD_MV | TFT_MAD_MX | TFT_MAD_COLOR_ORDER);
	      _width  = _init_height;
	      _height = _init_width;
	      break;
	    case 6:
	      writedata(TFT_MAD_COLOR_ORDER);
	      _width  = _init_width;
	      _height = _init_height;
	      break;
	    case 7:
	      writedata(TFT_MAD_MY | TFT_MAD_MV | TFT_MAD_COLOR_ORDER);
	      _width  = _init_height;
	      _height = _init_width;
	      break;
	  }

	  delayMicroseconds(10);

	  end_tft_write();

	  addr_row = 0xFFFF;
	  addr_col = 0xFFFF;

	  // Reset the viewport to the whole screen
	  resetViewport();
}

void TFT_Screen::setWindow(int32_t x0, int32_t y0, int32_t x1, int32_t y1)
{

  addr_row = 0xFFFF;
  addr_col = 0xFFFF;

  DC_C; tft_Write_8(TFT_CASET);
  DC_D; tft_Write_32C(x0, x1);
  DC_C; tft_Write_8(TFT_PASET);
  DC_D; tft_Write_32C(y0, y1);
  DC_C; tft_Write_8(TFT_RAMWR);
  DC_D;

}

void TFT_Screen::fillScreen(uint32_t color)
{
	fillRect(0, 0, _width, _height, color);
}
void TFT_Screen::fillRect(int32_t x, int32_t y, int32_t w, int32_t h, uint32_t color)
{
	 begin_tft_write();

	  setWindow(x, y, x + w - 1, y + h - 1);

	  DC_D;
	  pushBlock(color, w * h);
	  end_tft_write();

}
/***************************************************************************************
** Function name:           writecommand
** Description:             Send an 8 bit command to the TFT
***************************************************************************************/
void TFT_Screen::writecommand(uint8_t c)
{
  DC_C;
  tft_Write_8(c);
}


/***************************************************************************************
** Function name:           writedata
** Description:             Send a 8 bit data value to the TFT
***************************************************************************************/
void TFT_Screen::writedata(uint8_t d)
{
  DC_D;
  tft_Write_8(d);
}


void TFT_Screen::initLcd()
{

	  digitalWrite(TFT_REST,HIGH);
	  delay(5);
	  digitalWrite(TFT_REST,LOW);
	  delay(15);
	  digitalWrite(TFT_REST,HIGH);
	  delay(15);

	  digitalWrite(TFT_CS,HIGH);
	  digitalWrite(TFT_WR,HIGH);
	  digitalWrite(TFT_CS,LOW);  //CS

	   writecommand(0xF0);
	  writedata(0xC3);
	  writecommand(0xF0);
	  writedata(0x96);
	  writecommand(0x36);
	  writedata(0x68);
	  writecommand(0x3A);
	  writedata(0x05);
	  writecommand(0xB0);
	  writedata(0x80);
	  writecommand(0xB6);
	  writedata(0x20);
	  writedata(0x02);
	  writecommand(0xB5);
	  writedata(0x02);
	  writedata(0x03);
	  writedata(0x00);
	  writedata(0x04);
	  writecommand(0xB1);
	  writedata(0x80);
	  writedata(0x10);
	  writecommand(0xB4);
	  writedata(0x00);
	  writecommand(0xB7);
	  writedata(0xC6);
	  writecommand(0xC5);
	  writedata(0x24);
	  writecommand(0xE4);
	  writedata(0x31);
	  writecommand(0xE8);
	  writedata(0x40);
	  writedata(0x8A);
	  writedata(0x00);
	  writedata(0x00);
	  writedata(0x29);
	  writedata(0x19);
	  writedata(0xA5);
	  writedata(0x33);
	  writecommand(0xC2);
	  writecommand(0xA7);

	  writecommand(0xE0);
	  writedata(0xF0);
	  writedata(0x09);
	  writedata(0x13);
	  writedata(0x12);
	  writedata(0x12);
	  writedata(0x2B);
	  writedata(0x3C);
	  writedata(0x44);
	  writedata(0x4B);
	  writedata(0x1B);
	  writedata(0x18);
	  writedata(0x17);
	  writedata(0x1D);
	  writedata(0x21);

	  writecommand(0XE1);
	  writedata(0xF0);
	  writedata(0x09);
	  writedata(0x13);
	  writedata(0x0C);
	  writedata(0x0D);
	  writedata(0x27);
	  writedata(0x3B);
	  writedata(0x44);
	  writedata(0x4D);
	  writedata(0x0B);
	  writedata(0x17);
	  writedata(0x17);
	  writedata(0x1D);
	  writedata(0x21);

	  writecommand(0X36);
	  writedata(0x08);
	  writecommand(0xF0);
	  writedata(0xC3);
	  writecommand(0xF0);
	  writedata(0x69);
	  writecommand(0X13);
	  writecommand(0X11);
	  writecommand(0X29);


}

void TFT_Screen::init()
{
	initLcd();

}

void TFT_Screen::resetViewport(void)
{
  // Reset viewport to the whole screen
  _xDatum = 0;
  _yDatum = 0;
  _vpX = 0;
  _vpY = 0;
  _vpW = _width;
  _vpH = _height;
  _xWidth  = _width;
  _yHeight = _height;

  _vpDatum = false;
  _vpOoB   = false;
}


//constructor
TFT_Screen::TFT_Screen(int16_t w, int16_t h)
{

	// The control pins are deliberately set to the inactive state (CS high) as setup()
	// might call and initialise other SPI peripherals which would could cause conflicts
	// if CS is floating or undefined.
	#ifdef TFT_CS
	  pinMode(TFT_CS, OUTPUT);
	  digitalWrite(TFT_CS, HIGH); // Chip select high (inactive)
	#endif

	// Configure chip select for touchscreen controller if present
	#ifdef TOUCH_CS
	  pinMode(TOUCH_CS, OUTPUT);
	  digitalWrite(TOUCH_CS, HIGH); // Chip select high (inactive)
	#endif

	#ifdef TFT_WR
	  pinMode(TFT_WR, OUTPUT);
	  digitalWrite(TFT_WR, HIGH); // Set write strobe high (inactive)
	#endif

	#ifdef TFT_DC
	  pinMode(TFT_DC, OUTPUT);
	  digitalWrite(TFT_DC, HIGH); // Data/Command high = data mode
	#endif

	#ifdef TFT_RST
	  if (TFT_RST >= 0) {
	    pinMode(TFT_RST, OUTPUT);
	    digitalWrite(TFT_RST, HIGH); // Set high, do not share pin with another SPI device
	  }
	#endif



	  // Make sure read is high before we set the bus to output
	  pinMode(TFT_RD, OUTPUT);
	  digitalWrite(TFT_RD, HIGH);

	  // Set TFT data bus lines to output
	  pinMode(TFT_D0, OUTPUT); digitalWrite(TFT_D0, HIGH);
	  pinMode(TFT_D1, OUTPUT); digitalWrite(TFT_D1, HIGH);
	  pinMode(TFT_D2, OUTPUT); digitalWrite(TFT_D2, HIGH);
	  pinMode(TFT_D3, OUTPUT); digitalWrite(TFT_D3, HIGH);
	  pinMode(TFT_D4, OUTPUT); digitalWrite(TFT_D4, HIGH);
	  pinMode(TFT_D5, OUTPUT); digitalWrite(TFT_D5, HIGH);
	  pinMode(TFT_D6, OUTPUT); digitalWrite(TFT_D6, HIGH);
	  pinMode(TFT_D7, OUTPUT); digitalWrite(TFT_D7, HIGH);

	  CONSTRUCTOR_INIT_TFT_DATA_BUS;

	  resetViewport();

	  _init_width  = _width  = w; // Set by specific xxxxx_Defines.h file or by users sketch
	  _init_height = _height = h; // Set by specific xxxxx_Defines.h file or by users sketch

	  inTransaction = false;

	  addr_row = 0xFFFF;
	  addr_col = 0xFFFF;

	 // _xpivot = 0;
	 // _ypivot = 0;

	  resetViewport();
}



