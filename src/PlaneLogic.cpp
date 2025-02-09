#include "PlaneLogic.h"
#include <Arduino.h>

PlaneLogic::PlaneLogic(JsonHandler* jsonHandler, InputManager* inputManager)
    : m_jsonHandler(jsonHandler), m_inputManager(inputManager) {}

void PlaneLogic::loadParameters() {
    Serial.println("Lade Flugzeug-Parameter...");
    // Hier können Parameter geladen werden
}

void PlaneLogic::update() {
    Serial.println("Flugzeug-Logik aktiv...");
    // Hier kommt die Steuerlogik hin
}
