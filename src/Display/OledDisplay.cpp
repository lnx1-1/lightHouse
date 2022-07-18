//
// Created by Linus on 18.07.2022.
//

#include "OledDisplay.h"

OledDisplay::OledDisplay(): display(SCREEN_X, SCREEN_Y, &Wire, -1) {

}

void OledDisplay::setup() {
    Wire.begin();
    display.begin(SSD1306_SWITCHCAPVCC, DISPLAY_ADDR);
    Serial.println("\nTesting Display");
    display.display();
    delay(1000);
    display.clearDisplay();
    display.display();
}

Adafruit_SSD1306& OledDisplay::getDisp() {
    return display;
}
