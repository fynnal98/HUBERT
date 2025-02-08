// JsonHandler.h
#ifndef JSONHANDLER_H
#define JSONHANDLER_H

#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <string>

class JsonHandler {
public:
    JsonHandler(const std::string& filePath);

    bool LoadJson();
    bool SaveJson();

    template <typename T>
    T GetValueByKey(const std::string& path, T defaultValue) {
        JsonVariant value = resolvePath(path);
        if (!value.isNull()) {
            return value.as<T>();
        }
        return defaultValue;
    }

    template <typename T>
    void UpdateValueByKey(const std::string& path, T value) {
        JsonVariant node = resolvePath(path, true);
        if (!node.isNull()) {
            node.set(value);
            SaveJson();
        }
    }

private:
    std::string m_filePath;
    ArduinoJson::StaticJsonDocument<2048> m_jsonDoc;
    JsonVariant resolvePath(const std::string& path, bool createMissing = false);
};

#endif
