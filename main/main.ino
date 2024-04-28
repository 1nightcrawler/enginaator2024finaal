
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>

#include "game1.h"

extern TFT_eSPI tft = TFT_eSPI();

void setup() {
    tft.init();
    tft.setRotation(0);
    tft.fillScreen(TFT_RED);
}

void loop() {
  gameLoop();
}

