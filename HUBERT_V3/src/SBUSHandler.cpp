#include "SBUSHandler.h"
#include "JsonHandler.h"
#include <Arduino.h>

// Externe JSON-Handler-Instanz (wird im Hauptprogramm initialisiert)
extern JsonHandler jsonHandler;

SBUSHandler::SBUSHandler() : channelData(bfs::SbusData::NUM_CH, 0), sbus(nullptr) {}

bool SBUSHandler::loadConfig() {
    // Lese die RX- und TX-Pins aus der JSON-Datei
    rxPin = jsonHandler.GetValueByKey<int>("config/system/input/sbus/rxPin", -1);
    txPin = jsonHandler.GetValueByKey<int>("config/system/input/sbus/txPin", -1);

    if (rxPin == -1 || txPin == -1) {
        Serial.println("Fehler: RX- oder TX-Pin nicht definiert.");
        return false;
    }

    Serial.printf("SBUS RX-Pin: %d, TX-Pin: %d\n", rxPin, txPin);
    return true;
}

void SBUSHandler::begin() {
    if (!loadConfig()) {
        Serial.println("Fehler beim Laden der SBUS-Konfiguration.");
        return;
    }

    // Erstelle das SBUS-Objekt dynamisch
    sbus = new bfs::SbusRx(&Serial2, rxPin, txPin, true);
    sbus->Begin();  // SBUS starten
    Serial.println("SBUS-Verbindung gestartet.");
}

void SBUSHandler::update() {
    if (sbus->Read()) {
        auto data = sbus->data();
        receivedData.clear();
        for (int i = 0; i < bfs::SbusData::NUM_CH; ++i) {
            channelData[i] = data.ch[i];
            receivedData += std::to_string(channelData[i]) + ",";
        }
    }
}

std::string SBUSHandler::getInput() {
    return receivedData;
}

int SBUSHandler::getChannelValue(int channel) {
    if (channel >= 0 && channel < bfs::SbusData::NUM_CH) {
        int value = channelData[channel - 1];
        return value;
    } else {
        return 0; 
    }
}
