#include <Arduino.h>

#include "CaptivePortal/CaptivePortal.h"

#define ACTIVITY_TIME 1000
#define GPIO_PIN_RELAIS 2
bool isRelaisOn = false;

unsigned long lastTick = millis();

CaptivePortal portal;

String indexHTML =
#include "../res/index.html"
;

void handleIndexHTML();

void handleSendMSG();

void handleTestLight();

void printAllArgs();

void checkForMessageInput();


void setupGPIO() {
    pinMode(GPIO_PIN_RELAIS, OUTPUT);
}


void handleTestLight() {
    digitalWrite(GPIO_PIN_RELAIS, 1);
    isRelaisOn = true;
    lastTick = millis();
    handleIndexHTML();
}

void handleSendMSG() {
    checkForMessageInput();
    handleIndexHTML();
}

void handleIndexHTML() {
    portal.sendHTML(indexHTML);
}


void checkForMessageInput() {
    String msg = portal.getWebServerArgument("Message");
    if (!msg.isEmpty()) {
        Serial.printf("New Message Received: %s\n", msg.c_str());
    }
}

void setup() {

    Serial.begin(115200);
    // write your initialization code here

    portal.setup();
    portal.registerWebHandler("/sendmsg", &handleSendMSG);
    portal.registerWebHandler("/index.html", &handleIndexHTML);
    portal.registerWebHandler("/testLight", &handleTestLight);
    portal.registerWebHandlerOnNotFound(&handleIndexHTML);
    portal.startCaptivePortal();

    setupGPIO();

    Serial.printf("Setup done\n");
}


void checkRelaisState() {
    if (isRelaisOn) {
        if ((millis() - lastTick) > ACTIVITY_TIME) {
            digitalWrite(GPIO_PIN_RELAIS, 0);
            isRelaisOn = false;
        }
    }
}

void loop() {
    checkRelaisState();
    portal.handleClients();
}