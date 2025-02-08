#include <Arduino.h>
#include <SPIFFS.h>
#include "JsonHandler.h"

JsonHandler jsonHandler("/config.json");

void setup() {
    Serial.begin(115200);
    delay(1000);

    // Lade die JSON-Datei
    if (!jsonHandler.LoadJson()) {
        Serial.println("Fehler beim Laden der JSON-Datei.");
        return;
    }
}

void loop() {
}
