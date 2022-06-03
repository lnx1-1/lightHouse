#include <Arduino.h>
#include <DNSServer.h>
#include <ESPmDNS.h>
#include <WebServer.h>
#include <WiFi.h>

#define ACTIVITY_TIME 1000
#define GPIO_PIN_RELAIS 2
bool isRelaisOn = false;
const byte HTTP_OK = 200;
const char *HTML_TEXT = "text/html";
const char *hostName = "LightHouse";
const char *apSSID = "LightHouse Wifi";
const byte DNS_Port = 53;
WebServer WebServer(80);
DNSServer dnsServer;

IPAddress apIP(172, 0, 0, 1);
IPAddress netMask(255, 255, 255, 0);

unsigned long lastTick = millis();

String indexHTML();

void handleIndexHTML();

void handleSendMSG();

void handleTestLight();

void printAllArgs();

void checkForMessageInput();

void setupWifi() {
    WiFiClass::setHostname(hostName);
}

void setupGPIO() {
    pinMode(GPIO_PIN_RELAIS, OUTPUT);
}

void setupAPMode() {
    WiFiClass::mode(WIFI_MODE_AP);
    WiFi.softAPConfig(apIP, apIP, netMask);
    WiFi.softAP(apSSID);
}

void setupWebServer() {
    bool dnsOK = dnsServer.start(DNS_Port, "*", apIP);
    if (!dnsOK) {
        Serial.printf("\nERROR - NO SOCKETS AVAILABLE\n");
    }
    WebServer.on("/index.html", &handleIndexHTML);
    WebServer.on("/sendmsg", &handleSendMSG);
    WebServer.on("/testLight", &handleTestLight);
    WebServer.onNotFound(&handleIndexHTML);
    WebServer.begin();
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
    WebServer.send(HTTP_OK, HTML_TEXT, indexHTML());
}

void printAllArgs() {
    int argCount = WebServer.args();

    Serial.printf("Args: %d\n", argCount);
    for (int i = 0; i < argCount; i++) {
        String argName = WebServer.argName(i);
        String argValue = WebServer.arg(i);
        Serial.printf("%s: [%s]\n", argName.c_str(), argValue.c_str());
    }
}

String getWebServerArgument(String arg) {
    // Nimmt nicht die ID sondern den Namen!!!
    String rawArg = WebServer.arg(arg);

    //    Serial.printf("RAW: %s\n", rawArg.c_str());
    // Muss vll gar nicht formatiert werden
    rawArg.replace("<", "&lt;");
    rawArg.replace(">", "&gt;");
    rawArg.substring(0, 200);

    //    Serial.printf("Formatted: %s\n", rawArg.c_str());
    return rawArg;
}

void checkForMessageInput() {
    String msg = getWebServerArgument("Message");
    if (!msg.isEmpty()) {
        Serial.printf("New Message Received: %s\n", msg.c_str());
    }
}

void setup() {

    Serial.begin(115200);
    // write your initialization code here

    setupWifi();
    setupAPMode();
    setupWebServer();
    setupGPIO();

    Serial.printf("Setup done\n");
}

String indexHTML() {
    return R"(<!DOCTYPE html>
<html>

<head>
  <title> LIGHTHOUSE </title>
  <meta name=viewport content="width=device-width,initial-scale=1">
  <style>
    article {
      background: #f2f2f2;
      padding: 1.3em;
    }

    body {
      color: #BF4408;
      background: #1b1b1b;
      font-family: Century Gothic, sans-serif;
      font-size: 18px;
      line-height: 24px;
      margin: 0;
      padding: 0;
    }

    div {
      padding: 0.5em;
    }

    h1 {
      margin: 0.5em 0 0 0;
      padding: 0.5em;
    }

    input {
      width: 100%;
      padding: 9px 10px;
      margin: 8px 0;
      box-sizing: border-box;
      border-radius: 0;
      border: 1px solid #555555;
    }

    label {
      color: rgb(81, 33, 33);
      display: block;
      font-style: italic;
      font-weight: bold;
    }

    .switch {
      position: relative;
      display: inline-block;
      width: 50px;
      height: 25px;
    }

    /* Hide default HTML checkbox */
    .switch input {
      opacity: 0;
      width: 0;
      height: 0;
    }

    /* The slider */
    .slider {
      position: absolute;
      cursor: pointer;
      top: 0;
      left: 0;
      right: 0;
      bottom: 0;
      background-color: #ccc;
      -webkit-transition: .4s;
      transition: .4s;
    }

    .slider:before {
      position: absolute;
      content: "";
      height: 15px;
      width: 15px;
      left: 4px;
      bottom: 4px;
      background-color: white;
      -webkit-transition: .4s;
      transition: .4s;
    }

    input:checked+.slider {
      background-color: #2196f3;
    }

    input:focus+.slider {
      box-shadow: 0 0 1px #2196F3;
    }

    input[type=submit]{
      background-color: #1b1b1b;
      border-radius: 10px;
      width: 200px;
      color: #2196F3;
      font-weight: 900;
      border-color: #2196F3;
      /*border-style: ridge;*/
      border-width: 2px;
    }

    input[type=submit]:hover {
      border-style: ridge;
    }

    input:checked+.slider:before {
      -webkit-transform: translateX(26px);
      -ms-transform: translateX(26px);
      transform: translateX(26px);
    }

    /* Rounded sliders */
    .slider.round {
      border-radius: 34px;
    }

    .slider.round:before {
      border-radius: 50%;
    }


    nav {
      background: #0066ff;
      color: #fff;
      display: block;
      font-size: 1.3em;
      padding: 1em;
    }

    nav b {
      display: block;
      font-size: 1.5em;
      margin-bottom: 0.5em;
    }

    textarea {
      width: 100%;
    }

    div.buttonDiv{
      align-content: center;
      text-align: center;
    }
  </style>

</head>

<body>
  <nav><b>Light House</b>
    Send Text as Morse Code
  </nav>

  <div style="padding-top: 50px;">
    <div style="
      font-size: 20px;
      padding: 10px;
      border: 1px lightgray;
      margin: 10px;">
      <form action="/sendmsg" method="post">
        <div style="text-align: center;"><b>- Message -</b></div>

        <input type="text" id="message" name="Message" value="">
        <div class="buttonDiv">
        <input type="submit" value="SEND" >
      </div>
      </form>

      <form action="/testLight" method="post">
        <div class="buttonDiv">
        <input type="submit" value="Test Light" >
      </div>
      </form>

    </div>
    <div style="text-align: center; padding-top: 100px;"><a>&#169; All rights reserved.</a></div>

</html>)";
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
    dnsServer.processNextRequest();
    WebServer.handleClient();
}