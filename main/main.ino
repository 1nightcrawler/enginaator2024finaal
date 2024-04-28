
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "menu.h"

TFT_eSPI tft = TFT_eSPI();

#include "game1.h"

extern TFT_eSPI tft = TFT_eSPI();

void setup() {
<<<<<<< HEAD
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
=======
    menu(tft);
>>>>>>> d0fffc30110d419ebd7e3627713b9e3e254a1e4a
}

void loop() {
  gameLoop();
}

