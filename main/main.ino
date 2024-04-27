#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "menu.h"
#include "input.h"

extern TFT_eSPI tft = TFT_eSPI();

extern int DISPLAY_WIDTH = 240;
extern int DISPLAY_HEIGHT = 320;

extern bool mainMenu = true;
extern bool settingsMenu = false;
extern bool scoreboardMenu = false;
extern bool game1 = false;

int piezoPin = 4;


void setup() {
    Serial.begin(115200);

    initializeInput();

    pinMode(piezoPin, OUTPUT);

    tft.begin();
    tft.setRotation(0);
    tft.setViewport(0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
}

void loop() {
    if (mainMenu || settingsMenu || scoreboardMenu)
    {
    menu();
    }
}