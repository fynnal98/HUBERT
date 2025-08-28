#include "DroneLogic.h"
#include <Arduino.h>

DroneLogic::DroneLogic(JsonHandler* jsonHandler, InputManager* inputManager)
    : m_jsonHandler(jsonHandler), m_inputManager(inputManager) {}

void DroneLogic::loadParameters() {
    Serial.println("Lade Drohnen-Parameter...");
    // Hier können Parameter geladen werden
}

void DroneLogic::update() {
    Serial.println("Drohnen-Logik aktiv...");
    // Hier kommt die Steuerlogik hin
}
