#include "ConfigManager.h"
#include "JsonHandler.h"
#include <ArduinoJson.h>
#include <Arduino.h>

ConfigManager::ConfigManager(JsonHandler* jsonHandler) : m_jsonHandler(jsonHandler) {}

void ConfigManager::applyConfiguration(const std::vector<uint8_t>& payload) {
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, std::string(payload.begin(), payload.end()));

    if (error) {
        Serial.print("Fehler beim Parsen der Konfigurationsnachricht: ");
        Serial.println(error.c_str());
        return;
    }

    // Beispiel: Aktualisierung eines PID-Werts in der Konfiguration
    if (doc.containsKey("heli")) {
        JsonVariant heliConfig = m_jsonHandler->resolvePath("heli");
        heliConfig["pid"]["kp"] = doc["heli"]["pid"]["kp"].as<float>();
        heliConfig["pid"]["ki"] = doc["heli"]["pid"]["ki"].as<float>();
        heliConfig["pid"]["kd"] = doc["heli"]["pid"]["kd"].as<float>();

        Serial.println("PID-Parameter der Heli-Konfiguration aktualisiert.");
    }

    // Speichern der aktualisierten JSON-Konfiguration
    m_jsonHandler->SaveJson();
}
