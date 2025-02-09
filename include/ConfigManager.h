#pragma once
#include "JsonHandler.h"
#include <vector>
#include <ArduinoJson.h>

class ConfigManager {
public:
    ConfigManager(JsonHandler* jsonHandler);

    // Verarbeitet die eingehende Konfigurationsnachricht
    void processConfigUpdate(const std::vector<uint8_t>& payload);

private:
    JsonHandler* m_jsonHandler;

    // Wendet Änderungen aus der JSON-Payload an
    void applyChanges(const JsonVariant& updates);

    // Prüft, ob sich ein neuer Wert vom aktuellen unterscheidet
    bool areValuesEqual(const JsonVariant& currentValue, const JsonVariant& newValue);
};
