//
// Created by Linus on 07.06.2022.
//

#include "CaptivePortal.h"

#include <utility>


void CaptivePortal::setupWifi() {
    WiFiClass::setHostname(hostName);
}

void CaptivePortal::setupAP() {
    WiFiClass::mode(WIFI_MODE_AP);
    WiFi.softAPConfig(_apIP, _apIP, _netMask);
    WiFi.softAP(apSSID);
}

void CaptivePortal::setup(){
    setupWifi();
    setupAP();
}


void CaptivePortal::handleClients(){
    _dnsServer.processNextRequest();
    _webServer.handleClient();
}

void CaptivePortal::startCaptivePortal() {
    bool dnsOK = _dnsServer.start(DNS_Port, "*", _apIP);
    if (!dnsOK) {
        Serial.printf("\nERROR - NO SOCKETS AVAILABLE\n");
    }
    if(_handlerCount == 0){
        Serial.printf("WARNING: No Handlers Attached");
    } else {
        Serial.printf("Starting WebServer with %d handlers attached\n",_handlerCount);
    }

    _webServer.begin();
}

void CaptivePortal::registerWebHandler(const String& onPage, void (*handlerFunction)()) {
    _webServer.on(onPage,handlerFunction);
    _handlerCount++;
}

void CaptivePortal::registerWebHandlerOnNotFound(void (*handlerFunction)()) {
    _webServer.onNotFound(handlerFunction);
    _handlerCount++;
}

void CaptivePortal::sendHTML(const String& html_text) {
    _webServer.send(HTTP_OK, HTML_TEXT,html_text);
}

void CaptivePortal::printAllArgs() {
    int argCount = _webServer.args();

    Serial.printf("Args: %d\n", argCount);
    for (int i = 0; i < argCount; i++) {
        String argName = _webServer.argName(i);
        String argValue = _webServer.arg(i);
        Serial.printf("%s: [%s]\n", argName.c_str(), argValue.c_str());
    }
}

String CaptivePortal::getWebServerArgument(String arg) {
    // Nimmt nicht die ID sondern den Namen!!!
    String rawArg = _webServer.arg(std::move(arg));

    //    Serial.printf("RAW: %s\n", rawArg.c_str());
    // Muss vll gar nicht formatiert werden
    rawArg.replace("<", "&lt;");
    rawArg.replace(">", "&gt;");
    rawArg.substring(0, 200);

    //    Serial.printf("Formatted: %s\n", rawArg.c_str());
    return rawArg;
}





