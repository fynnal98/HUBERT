#pragma once
#include "JsonHandler.h"
#include <vector>
#include <ArduinoJson.h>

class ConfigManager {
public:
    ConfigManager(JsonHandler* jsonHandler);
    void processConfigUpdate(const std::vector<uint8_t>& payload);

private:
    JsonHandler* m_jsonHandler;
    void applyChanges(const JsonVariant& updates);
};
