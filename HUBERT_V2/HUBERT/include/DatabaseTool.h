#ifndef DATABASETOOL_H
#define DATABASETOOL_H

#include <ArduinoJson.h>
#include <SPIFFS.h>
#include <string>

class DatabaseTool {
public:
    void init();  // Initialisiert das DatabaseTool und lädt die JSON-Datei    
    template <typename T>
    T get(const char* path, T defaultValue);  // Gibt den Wert zu einem JSON-Pfad zurück

    template <typename T>
    void set(const char* path, T value);  // Setzt einen Wert in der JSON-Datenbank
    JsonVariant resolvePath(const char* path, bool createMissing = false);
    void saveToFile();

private:
    StaticJsonDocument<2048> jsonDoc;  // Speicher für die JSON-Daten

};

// Template-Methoden müssen in der Header-Datei definiert sein
template <typename T>
T DatabaseTool::get(const char* path, T defaultValue) {
    JsonVariant value = resolvePath(path);
    if (!value.isNull()) {
        return value.as<T>();
    }
    return defaultValue;  // Rückgabe eines Standardwerts
}

template <typename T>
void DatabaseTool::set(const char* path, T value) {
    JsonVariant node = resolvePath(path, true);
    if (!node.isNull()) {
        node.set(value);
        saveToFile();
    }
}

#endif
