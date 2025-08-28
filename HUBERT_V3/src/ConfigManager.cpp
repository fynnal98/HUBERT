#include "ConfigManager.h"
#include <Arduino.h>

ConfigManager::ConfigManager(JsonHandler* jsonHandler)
    : m_jsonHandler(jsonHandler) {}

void ConfigManager::processConfigUpdate(const std::vector<uint8_t>& payload) {
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, std::string(payload.begin(), payload.end()));

    if (error) {
        Serial.print("ConfigManager: Fehler beim Parsen der JSON-Daten: ");
        Serial.println(error.c_str());
        return;
    }

    applyChanges(doc.as<JsonVariant>());

    // Speichern der aktualisierten Konfiguration
    m_jsonHandler->SaveJson();
}

void ConfigManager::applyChanges(const JsonVariant& updates) {
    for (JsonPair kv : updates.as<JsonObject>()) {
        const char* key = kv.key().c_str();
        JsonVariant newValue = kv.value();

        // Bestehenden Wert ermitteln
        JsonVariant currentValue = m_jsonHandler->resolvePath(key);

        // Wenn der aktuelle Wert nicht existiert oder abweicht, wird aktualisiert
        if (currentValue.isNull() || !areValuesEqual(currentValue, newValue)) {
            currentValue.set(newValue);
            Serial.printf("Parameter '%s' wurde geändert.\n", key);
        } else {
            Serial.printf("Parameter '%s' ist bereits aktuell.\n", key);
        }
    }
}

// Hilfsfunktion zum Vergleichen der alten und neuen Werte
bool ConfigManager::areValuesEqual(const JsonVariant& currentValue, const JsonVariant& newValue) {
    if (currentValue.is<float>() && newValue.is<float>()) {
        return currentValue.as<float>() == newValue.as<float>();
    } else if (currentValue.is<int>() && newValue.is<int>()) {
        return currentValue.as<int>() == newValue.as<int>();
    } else if (currentValue.is<std::string>() && newValue.is<std::string>()) {
        return currentValue.as<std::string>() == newValue.as<std::string>();
    } else if (currentValue.is<bool>() && newValue.is<bool>()) {
        return currentValue.as<bool>() == newValue.as<bool>();
    }
    return false;  // Unterschiedliche oder nicht unterstützte Typen
}
