
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "menu.h"

TFT_eSPI tft = TFT_eSPI();

void setup() {
    menu(tft);
}

void loop() {
}