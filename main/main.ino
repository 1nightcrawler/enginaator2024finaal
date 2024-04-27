#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "menu.h"
#include "game1.h"

extern TFT_eSPI tft = TFT_eSPI();

void setup() {
    gameSetup();
    //menu(tft);
}


void loop() {
  gameLoop();
}
