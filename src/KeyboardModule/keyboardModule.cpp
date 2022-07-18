//
// Created by Linus on 18.07.2022.
//

#include "keyboardModule.h"


/* Keyboard constants  Change to suit your Arduino
   define pins used for data and clock from keyboard */
#define DATAPIN 33 //
#define IRQPIN  32


void keyboardModule::setup() {
// Configure the keyboard library
    Serial.println("PS2 Setting Up Keyboard");
    keyboard.begin(DATAPIN, IRQPIN);
//    Serial.begin(115200);
    keyboard.setNoRepeat(1);
    keyboard.setNoBreak(1);

}

String keyboardModule::getBuffer() {
    return textBuffer;
}

bool keyboardModule::isUpdateAvailable() {
    bool outVal = isUpdate;
    isUpdate = false;
    return outVal;
}

void keyboardModule::loop() {
    if (keyboard.available()) {
        // read the next key
        uint16_t c = keyboard.read();
        if (c > 0) {
            isUpdate = true;
//            Serial.print(" - Status Bits ");
//            Serial.print(c >> 8, HEX);
//            Serial.print("  Code ");
//            Serial.println(c & 0xFF, HEX);

            uint8_t realChar = (uint8_t) (c & 0xFF);
            if (realChar == 'Y') {
                realChar = 'Z';
            } else if (realChar == 'Z') {
                realChar = 'Y';
            }
            if ((realChar >= 'A' && realChar <= 'Z') || (realChar >= '0' && realChar <= '9')) {
//                Serial.printf("%c", realChar);
                textBuffer += (char) realChar;
//                textBuffer.concat(realChar);

            }

            switch (realChar) {
                case PS2_KEY_ENTER:
                    Serial.println("\nEnter");
                    Serial.println(textBuffer);
                    textBuffer = "";
                    break;
                case PS2_KEY_DELETE:
                    break;
                case PS2_KEY_SPACE:
                    textBuffer.concat(' ');
                    break;
                case 0x1C: //BACKSPACE
                    textBuffer = textBuffer.substring(0, textBuffer.length() - 1);
                    break;
            }

        }
    }
}