#include "Arduino.h"
#include <TFT_lib.h>

TFT_Screen tft = TFT_Screen();

//The setup function is called once at startup of the sketch
void setup()
{

  tft.init();
  tft.setRotation(1);

}

// The loop function is called in an endless loop

void loop() {

  tft.fillScreen(TFT_BLACK);
  // Draw some random circles
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.fillEllipse(x, y, rx, ry, random(0xFFFF));

  }

  delay(2000);

  tft.fillScreen(TFT_BLACK);

  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(480 - rx - rx);
    int y = ry + random(320 - ry - ry);
    tft.drawEllipse(x, y, rx, ry, random(0xFFFF));

  }

  delay(2000);


}
