#include "WiFiInput.h"
#include "InputHandler.h"
#include <Arduino.h>

void WiFiInput::init(DatabaseTool* db, InputHandler* inputHandler) {
    this->database = db;
    this->inputHandler = inputHandler;
    setupWiFi(db);
}

void WiFiInput::setupWiFi(DatabaseTool* db) {
    const char* ssid = db->get<const char*>("wifi/ssid", "HUBERT");
    const char* password = db->get<const char*>("wifi/password", "12345678");
    int port = db->get<int>("wifi/port", 4210);

    WiFi.softAP(ssid, password);
    udp.begin(port);
    Serial.printf("WiFiInput: WLAN gestartet: SSID = %s, Port = %d\n", ssid, port);
}

void WiFiInput::update() {
    int packetSize = udp.parsePacket();
    if (packetSize > 0) {
        char incomingPacket[255];
        int len = udp.read(incomingPacket, 255);
        incomingPacket[len] = '\0';
        receivedData = std::string(incomingPacket);
        processAppData(receivedData);
    }
}

void WiFiInput::processAppData(const std::string& data) {
    if (data.find('=') != std::string::npos) {
        Serial.println("WiFiInput: Parameterdaten empfangen.");
        // Hier könnten Parameterdaten direkt weitergeleitet werden, z. B. an ParameterUpdater
    } else {
        Serial.println("WiFiInput: Steuerdaten empfangen.");
        inputHandler->updateWithAppData(data);  // Steuerdaten an InputHandler weiterleiten
    }
}

std::string WiFiInput::getInput() {
    return receivedData;
}
