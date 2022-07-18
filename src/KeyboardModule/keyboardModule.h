//
// Created by Linus on 18.07.2022.
//

#ifndef TESTKEYBOARD_KEYBOARDMODULE_H
#define TESTKEYBOARD_KEYBOARDMODULE_H


#include <Arduino.h>

#include "PS2KeyAdvanced.h"

class keyboardModule {
private:
    PS2KeyAdvanced keyboard;
    String textBuffer = "";
    String sendBuffer = "";
    bool isUpdate = false;
    bool sendBuffAvail = false;
public:
    bool isUpdateAvailable();
    void setup();
    void loop();
    String getBuffer();
    String getSendBuffer();
    bool isSendBufferAvailable();
    void flushBuff();
};


#endif //TESTKEYBOARD_KEYBOARDMODULE_H
