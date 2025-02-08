#include "JsonHandler.h"

JsonHandler::JsonHandler(const std::string& filePath) : m_filePath(filePath) {}

bool JsonHandler::LoadJson() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS konnte nicht initialisiert werden");
        return false;
    }

    File file = SPIFFS.open(m_filePath.c_str(), "r");
    if (!file) {
        Serial.println("Datei konnte nicht geöffnet werden");
        return false;
    }

    DeserializationError error = deserializeJson(m_jsonDoc, file);
    file.close();

    if (error) {
        Serial.print("Fehler beim Deserialisieren: ");
        Serial.println(error.c_str());
        return false;
    }

    Serial.println("Json wurde geladen");
    return true;
}

bool JsonHandler::SaveJson() {
    File file = SPIFFS.open(m_filePath.c_str(), "w");
    if (!file) {
        Serial.println("Datei konnte nicht geöffnet werden");
        return false;
    }

    serializeJson(m_jsonDoc, file);
    file.close();
    return true;
}

JsonVariant JsonHandler::resolvePath(const std::string& path, bool createMissing) {
    JsonVariant node = m_jsonDoc.as<JsonVariant>();

    std::string pathCopy = path;  
    char* token = strtok(&pathCopy[0], "/");

    while (token && node) {
        if (!node.containsKey(token) && createMissing) {
            node.createNestedObject(token);
        }
        node = node[token];
        token = strtok(NULL, "/");
    }
    return node;
}

