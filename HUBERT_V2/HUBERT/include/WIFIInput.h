#ifndef WIFIINPUT_H
#define WIFIINPUT_H

#include "DatabaseTool.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string>

// Vorwärtsdeklaration des InputHandlers zur Vermeidung zyklischer Abhängigkeiten
class InputHandler;

class WiFiInput {
public:
    void init(DatabaseTool* db, InputHandler* inputHandler);
    void update();  // Prüft UDP-Daten und verarbeitet Steuer- oder Parameterdaten
    std::string getInput();  // Gibt die zuletzt empfangenen Daten zurück

private:
    WiFiUDP udp;
    DatabaseTool* database;
    InputHandler* inputHandler;
    std::string receivedData;

    void setupWiFi(DatabaseTool* db);
    void processAppData(const std::string& data);  // Entscheidet, ob es Steuer- oder Parameterdaten sind
};

#endif  // WIFIINPUT_H
