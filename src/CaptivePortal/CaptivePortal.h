//
// Created by Linus on 07.06.2022.
//

#ifndef LIGHTHOUSE_CAPTIVEPORTAL_H
#define LIGHTHOUSE_CAPTIVEPORTAL_H


#include <cstdint>
#include <WebServer.h>
#include <DNSServer.h>
#include <iostream>

#define ACCESPOINT_IP 172, 0, 0, 1
#define ACCESPOINT_NETMASK 255, 255, 255, 0
#define HOST_NAME "LightHouse"
#define AP_SSID "LightHouse Wifi"



class CaptivePortal {

private:
    const uint8_t HTTP_OK = 200;
    const char *HTML_TEXT = "text/html";
    const char *hostName = HOST_NAME;
    const char *apSSID = AP_SSID;
    const uint8_t DNS_Port = 53;
    WebServer _webServer;
    DNSServer _dnsServer;
    IPAddress _apIP;
    IPAddress _netMask;


    uint32_t _handlerCount = 0;

    void setupWifi();
    void setupAP();
    void printAllArgs();

public:
    CaptivePortal() :
            _webServer(80),
            _apIP(ACCESPOINT_IP),
            _netMask(ACCESPOINT_NETMASK){
    }
    void startCaptivePortal();
    void sendHTML(const String&);
    void registerWebHandler(const String& onPage, void(*handlerFunction)());
    void registerWebHandlerOnNotFound(void(*handlerFunction)());
    void setup();
    String getWebServerArgument(String arg);

    /**
     * Has to be Called in Every Loop iteration (works Synchron)
     */
    void handleClients();
};


#endif //LIGHTHOUSE_CAPTIVEPORTAL_H
