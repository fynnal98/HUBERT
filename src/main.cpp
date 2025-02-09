#include <Arduino.h>
#include <SPIFFS.h>

#include "JsonHandler.h"
#include "WiFiHandler.h"
#include "SBUSHandler.h"
#include "ControlManager.h"
#include "SensorHandler.h"

JsonHandler jsonHandler("/config.json");
WiFiHandler wifiHandler;
SBUSHandler sbusHandler;
ControlManager controlManager(&jsonHandler); 
SensorHandler sensorHandler;



void setup() {
    Serial.begin(115200);

    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS konnte nicht initialisiert werden.");
        return;
    }

    if (!jsonHandler.LoadJson()) {
        Serial.println("Fehler beim Laden der JSON-Datei.");
        return;
    }
    Serial.println("JSON erfolgreich geladen.");


    sbusHandler.begin();
    wifiHandler.begin();
    controlManager.begin();
    sensorHandler.begin(&jsonHandler);
}


void loop() {
    controlManager.update();
    sensorHandler.update();
    sbusHandler.update(); 
    wifiHandler.handleClients();
    delay (100);
}
