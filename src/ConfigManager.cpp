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
        JsonVariant value = kv.value();

        JsonVariant target = m_jsonHandler->resolvePath(key);
        if (!target.isNull()) {
            target.set(value);
            Serial.printf("Parameter '%s' aktualisiert.\n", key);
        } else {
            Serial.printf("Parameter '%s' nicht gefunden.\n", key);
        }
    }
}
