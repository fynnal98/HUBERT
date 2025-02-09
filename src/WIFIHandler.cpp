#include "WiFiHandler.h"
#include "JsonHandler.h"
#include <Arduino.h>

// Externe JSON-Handler-Instanz (wird im Hauptprogramm initialisiert)
extern JsonHandler jsonHandler;

WiFiHandler::WiFiHandler() {}

void WiFiHandler::begin() {
    if (!loadConfig()) {
        Serial.println("Fehler beim Laden der WLAN-Konfiguration.");
        return;
    }

    startAccessPoint();
    m_udpServer.begin(m_port);
    Serial.println("UDP-Server gestartet. Warte auf Nachrichten...");
}

bool WiFiHandler::loadConfig() {
    m_ssid = jsonHandler.GetValueByKey<std::string>("config/network/wifi/ssid", "DefaultAP");
    m_password = jsonHandler.GetValueByKey<std::string>("config/network/wifi/password", "defaultpass");
    m_port = jsonHandler.GetValueByKey<uint16_t>("config/network/wifi/port", 4210);

    if (m_ssid.empty() || m_password.empty()) {
        Serial.println("SSID oder Passwort ist leer. Standardwerte werden verwendet.");
        return false;
    }

    Serial.println(("SSID: " + m_ssid).c_str());
    Serial.println(("Passwort: " + m_password).c_str());
    Serial.println("Port: " + String(m_port));
    return true;
}

void WiFiHandler::startAccessPoint() {
    Serial.println("Starte Access Point...");
    WiFi.softAP(m_ssid.c_str(), m_password.c_str());
    printNetworkDetails();
}

void WiFiHandler::printNetworkDetails() {
    Serial.println("Access Point gestartet.");
    Serial.println(("SSID: " + m_ssid).c_str());
    Serial.println(("IP-Adresse: " + WiFi.softAPIP().toString()).c_str());
}

void WiFiHandler::handleClients() {
    int packetSize = m_udpServer.parsePacket();
    if (packetSize) {
        Serial.printf("Empfangenes Paket, Größe: %d Bytes\n", packetSize);

        // Puffer für die eingehende Nachricht
        char incomingPacket[255];
        int len = m_udpServer.read(incomingPacket, 255);
        if (len > 0) {
            incomingPacket[len] = 0;  // Null-terminierte Nachricht
        }

        Serial.printf("Empfangene Nachricht: %s\n", incomingPacket);

        // Antwort senden
        m_udpServer.beginPacket(m_udpServer.remoteIP(), m_udpServer.remotePort());
        m_udpServer.write((const uint8_t*)"ESP32 hat Ihre Nachricht erhalten.", strlen("ESP32 hat Ihre Nachricht erhalten."));
        m_udpServer.endPacket();
    } else {
        // Serial.println("Warte auf eingehende UDP-Pakete...");
    }
}
