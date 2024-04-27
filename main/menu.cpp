#include "menu.h"
#include "input.h"
#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>


extern int DISPLAY_WIDTH;
extern int DISPLAY_HEIGHT;

extern TFT_eSPI tft;

int gridX = 2;
int gridY = 2;
int cursorX = 1;
int cursorY = 1;

int lastcursorX = 1;
int lastcursorY = 1;

extern bool mainMenu;
extern bool settingsMenu;
extern bool scoreboardMenu;
extern bool game1;

void menu() {
    mainMenuActions();

    int joystickMoveDirection = joystickXYInput();

    if (joystickMoveDirection == 2 && cursorX < 2) {
        cursorX++;
        // Serial.println("moveRight");
    }
    if (joystickMoveDirection == 4 && cursorX > 1) {
        cursorX--;
        // Serial.println("moveLeft");
    }
    if (joystickMoveDirection == 3 && cursorY > 1) {
        cursorY--;
        // Serial.println("moveUp");
    }
    if (joystickMoveDirection == 1 && cursorY < 2) {
        cursorY++;
        // Serial.println("moveDown");
    }

    if (cursorX != lastcursorX || cursorY != lastcursorY) {
        lastcursorX = cursorX;
        lastcursorY = cursorY;

        if (mainMenu) {
            drawMainMenu();
        }
        else if (settingsMenu) {
            drawSettingsMenu();
        }
        else if (scoreboardMenu) {
            drawScoreboardMenu();
        }
    }
}

void drawMainMenu() {    
    int rectWidth = DISPLAY_WIDTH / gridX;
    int rectHeight = DISPLAY_HEIGHT / gridY;
    tft.setTextColor(TFT_BLACK);

    // menu item 1 (settings)
    if (cursorX == 1 && cursorY == 1) {
        tft.fillRect(0, 0, rectWidth, rectHeight, TFT_GREEN);
    } else {
        tft.fillRect(0, 0, rectWidth, rectHeight, TFT_WHITE);
    }

    tft.drawString("Settings", 10, 10);

    // menu item 2 (scoreboard)
    if (cursorX == 2 && cursorY == 1) {
        tft.fillRect(rectWidth, 0, rectWidth, rectHeight, TFT_GREEN);
    } else {
        tft.fillRect(rectWidth, 0, rectWidth, rectHeight, TFT_WHITE);
    }

    tft.drawString("Scoreboard", 10 + rectWidth, 10);

    // menu item 3 (play)
    if (cursorX == 1 && cursorY == 2) {
        tft.fillRect(0, rectHeight, rectWidth, rectHeight, TFT_GREEN);
    } else {
        tft.fillRect(0, rectHeight, rectWidth, rectHeight, TFT_WHITE);
    }

    tft.drawString("Play", 10, 10 + rectHeight);

    // menu item 4 (empty)
    if (cursorX == 2 && cursorY == 2) {
        tft.fillRect(rectWidth, rectHeight, rectWidth, rectHeight, TFT_GREEN);
    } else {
        tft.fillRect(rectWidth, rectHeight, rectWidth, rectHeight, TFT_WHITE);
    }

    tft.drawString("Empty", 10 + rectWidth, 10 + rectHeight);
}

void drawSettingsMenu() {
    tft.fillScreen(TFT_NAVY);
}

void drawScoreboardMenu() {
    tft.fillScreen(TFT_RED);
}

void mainMenuActions() {
    int joystickButton = joystickButtonInput();

    if (cursorX == 1 && cursorY == 1 && joystickButton == 0) {
        // settings
        mainMenu = false;
        settingsMenu = true;
        scoreboardMenu = false;
        drawSettingsMenu();
    }
    if (cursorX == 2 && cursorY == 1 && joystickButton == 0) {
        // scoreboard
        mainMenu = false;
        settingsMenu = false;
        scoreboardMenu = true;
        drawScoreboardMenu();
    }
    if (cursorX == 1 && cursorY == 2 && joystickButton == 0) {
        // play
        mainMenu = false;
        settingsMenu = false;
        scoreboardMenu = false;
        game1 = true;
    }
    if (cursorX == 2 && cursorY == 2) {
        // empty
    }
}
