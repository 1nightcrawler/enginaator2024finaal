#include "menu.h"


void menu(TFT_eSPI tft) {
    tft.begin();
    tft.setRotation(1);
    tft.setViewport(0, 0, 240, 320);
    tft.fillScreen(TFT_NAVY);
    tft.fillRect(-160, -160, 320, 500, TFT_DARKGREY);
}

