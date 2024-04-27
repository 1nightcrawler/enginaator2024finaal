#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include "input.h"

// joystick
int joystickXPin = 6;
int joystickYPin = 7;
int joystickButtonPin = 15;

int joystickXValue = 0;
int joystickYValue = 0;
int joystickButtonValue = 0;

bool moveRight = false;
bool moveLeft = false;
bool moveUp = false;
bool moveDown = false;

int moveDirection = 0;

int joystickXYInput() 
{
    joystickXValue = analogRead(joystickXPin);
    joystickYValue = analogRead(joystickYPin);

    if (joystickXValue < 250) {
        moveLeft = true;
    }
    else {
        moveLeft = false;
    }

    if (joystickXValue > 3750) {
        moveRight = true;
    }
    else {
        moveRight = false;
    }
    if (joystickYValue < 250) {
        moveUp = true;
    }
    else {
        moveUp = false;
    }

    if (joystickYValue > 3750) {
        moveDown = true;
    }
    else {
        moveDown = false;
    }

    if (moveUp) {
        moveDirection = 1;
    }
    else if (moveRight) {
        moveDirection = 2;
    }
    else if (moveDown) {
        moveDirection = 3;
    }
    else if (moveLeft) {
        moveDirection = 4;
    }
    else {
        moveDirection = 0;
    }

    return moveDirection;
}

int joystickButtonInput() 
{
    joystickButtonValue = digitalRead(joystickButtonPin);

    return joystickButtonValue;
}

// button1
int btn1Pin = 16;

int btn1Value = 0;

int button1Input() 
{
    btn1Value = digitalRead(btn1Pin);

    return btn1Value;
}

// button2
int btn2Pin = 17;

int btn2Value = 0;

int button2Input() 
{
    btn2Value = digitalRead(btn2Pin);

    return btn2Value;
}

// button3
int btn3Pin = 18;

int btn3Value = 0;

int button3Input() 
{
    btn3Value = digitalRead(btn3Pin);

    return btn3Value;
}


void initializeInput()
{
    pinMode(joystickXPin, INPUT);
    pinMode(joystickYPin, INPUT);
    pinMode(joystickButtonPin, INPUT);

    pinMode(btn1Pin, INPUT_PULLUP);
    pinMode(btn2Pin, INPUT_PULLUP);
    pinMode(btn3Pin, INPUT_PULLUP);
}