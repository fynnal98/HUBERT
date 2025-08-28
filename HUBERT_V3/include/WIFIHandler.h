#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>

class WiFiHandler {
public:
    WiFiHandler();  // Konstruktor ohne Parameter

    void begin();  // Startet den Access Point und den UDP-Server
    void handleClients();  // Behandelt eingehende UDP-Pakete

private:
    std::string m_ssid;
    std::string m_password;
    uint16_t m_port;
    WiFiUDP m_udpServer;

    bool loadConfig();  // Lädt SSID, Passwort und Port aus der JSON-Konfiguration
    void startAccessPoint();  // Startet den WLAN-Access Point
    void printNetworkDetails();  // Gibt Details zum Access Point aus
};

#endif
