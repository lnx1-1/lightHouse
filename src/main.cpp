#include <Arduino.h>

#include "CaptivePortal/CaptivePortal.h"
#include "MorseStation/MorseStation.h"
#include <regex>

#define ACTIVITY_TIME 1000
#define GPIO_PIN_RELAIS 2
bool isRelaisOn = false;
uint64_t currentMorseImpulsEnd = 0;
unsigned long lastTick = millis();
uint32_t _DIT_lenMS = DIT_LEN;
uint32_t _DAH_lenMS = DAH_LEN;
CaptivePortal portal;
MorseStation morse_station;

string indexHTML =

#include "../res/index.html"
        ;

void handleIndexHTML();

void updateSpeed();

string changeSpeedValue(string html, int newVal);

void handleSendMSG();

void handleTestLight();

void printAllArgs();

String getMessage();


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
    updateSpeed();
    String msg = getMessage();
    morse_station.addMessage(msg.c_str());
    handleIndexHTML();
}

string changeSpeedValue(string html, int newVal) {
    regex reg("(name=\"morseSpeed\" value=)\"\\d*(\".*?)");
    string replacementString = "$1\"" + to_string(newVal) + "$2";
    html = regex_replace(html, reg, replacementString);

    string replacementString2 = "$1 " + to_string(newVal) + "$2";
    regex reg2("(<div style = \"text-align: center;\">Speed:).*?(</div>).*?");
    html = regex_replace(html, reg2, replacementString2);
    return html;
}

void handleIndexHTML() {
    indexHTML = changeSpeedValue(indexHTML, (int)_DIT_lenMS);
    portal.sendHTML(indexHTML.c_str());
}


String getMessage() {
    String msg = portal.getWebServerArgument("Message");
    if (!msg.isEmpty()) {
        Serial.printf("New Message Received: %s\n", msg.c_str());
    }
    return msg;
}


void updateSpeed() {
    String spd = portal.getWebServerArgument("morseSpeed");
    int outSpd = stoi(spd.c_str());
    if (outSpd >= 100 && outSpd <= 2000) {
        cout << "Go New Speed: " << outSpd << endl;
        _DIT_lenMS = outSpd;
        _DAH_lenMS = _DIT_lenMS * 3;
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

void relaisSwitch(bool state) {
    digitalWrite(GPIO_PIN_RELAIS, state);
    isRelaisOn = state;
}

void checkMorseQueue() {
    if (millis() > currentMorseImpulsEnd) {
        relaisSwitch(false);
        if (morse_station.isDataReady()) {
            char impuls = morse_station.getNextImpuls();

            uint32_t impulsLen = 0;
            switch (impuls) {
                case '.':
                    impulsLen = _DIT_lenMS;
                    relaisSwitch(true);
                    break;
                case '-':
                    impulsLen = _DAH_lenMS;
                    relaisSwitch(true);
                    break;
                case DIT_PAUSE_SIGN:
                    impulsLen = _DIT_lenMS;
                    relaisSwitch(false);
                    break;
                default:
                    Serial.println("\nERROR WHILE READING IMPULS::\n");
            }
            Serial.printf("Handling Impuls %c with len %d, Relais State: %i\n", impuls, impulsLen, isRelaisOn);
            currentMorseImpulsEnd = millis() + impulsLen;
        }
    }


}

void loop() {
//    checkRelaisState();
    portal.handleClients();
    checkMorseQueue();
}