//
// Created by Linus on 18.07.2022.
//

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include "KeyboardModule/keyboardModule.h"


#define SCREEN_X 128
#define SCREEN_Y 64
#define DISPLAY_ADDR 0x3c

Adafruit_SSD1306 display(SCREEN_X, SCREEN_Y, &Wire, -1);

keyboardModule keyboard;

void setup()
{
    keyboard.setup();
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR);
    Serial.begin(9600);
    while (!Serial);             // Leonardo: wait for serial monitor
    Serial.println("\nTesting Display");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
    display.println("Test Text Hallo World");
}


void loop()
{
    keyboard.loop();
    if(keyboard.isUpdateAvailable()){
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0,0);
        display.println(keyboard.getBuffer());
        display.display();
        Serial.printf("%s\n", keyboard.getBuffer().c_str());
    }

}