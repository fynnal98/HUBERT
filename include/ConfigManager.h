#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <Arduino.h>
#include <vector>
#include <string>

class JsonHandler;

class ConfigManager {
public:
    ConfigManager(JsonHandler* jsonHandler);
    void applyConfiguration(const std::vector<uint8_t>& payload);

private:
    JsonHandler* m_jsonHandler;
};

#endif // CONFIG_MANAGER_H
