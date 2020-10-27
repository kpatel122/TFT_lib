#include "Arduino.h"
#include <TFT_lib.h>

TFT_Screen screen = TFT_Screen();

//The setup function is called once at startup of the sketch
void setup()
{

  screen.init();
}

// The loop function is called in an endless loop
void loop()
{

  screen.fillScreen(0xf800);
  screen.fillScreen(0x07E0);
  screen.fillScreen(0x001F);
}
