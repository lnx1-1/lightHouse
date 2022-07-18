//
// Created by Linus on 18.07.2022.
//

#ifndef LIGHTHOUSE_OLEDDISPLAY_H
#define LIGHTHOUSE_OLEDDISPLAY_H

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "KeyboardModule/keyboardModule.h"


#define SCREEN_X 128
#define SCREEN_Y 64
#define DISPLAY_ADDR 0x3c



class OledDisplay {
public: OledDisplay();
    void setup();
    Adafruit_SSD1306& getDisp();
private:
    Adafruit_SSD1306 display;


};


#endif //LIGHTHOUSE_OLEDDISPLAY_H
