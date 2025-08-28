#include "DatabaseTool.h"

void DatabaseTool::init() {
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS konnte nicht gestartet werden!");
        return;
    }

    // Prüfe, ob die Datei existiert
    if (!SPIFFS.exists("/database.json")) {
        Serial.println("Fehler: database.json nicht gefunden!");
        return;
    }

    // Datei öffnen
    File file = SPIFFS.open("/database.json", "r");
    if (!file) {
        Serial.println("Fehler beim Öffnen der Datei database.json!");
        return;
    }

    // Inhalt der Datei ausgeben
    Serial.println("Inhalt der database.json:");
    while (file.available()) {
        Serial.print((char)file.read());  // Byteweise ausgeben
    }
    Serial.println();

    // Datei wieder an den Anfang setzen und JSON-Daten einlesen
    file.seek(0);  // Setze den Dateizeiger zurück an den Anfang
    DeserializationError error = deserializeJson(jsonDoc, file);
    if (error) {
        Serial.print("Fehler beim Lesen der JSON-Datei: ");
        Serial.println(error.c_str());
        file.close();
        return;
    }

    file.close();
    Serial.println("JSON-Datei erfolgreich geladen.");
}

JsonVariant DatabaseTool::resolvePath(const char* path, bool createMissing) {
    JsonVariant node = jsonDoc.as<JsonVariant>();
    char* token = strtok(strdup(path), "/");
    while (token && node) {
        if (!node.containsKey(token) && createMissing) {
            node.createNestedObject(token);
        }
        node = node[token];
        token = strtok(NULL, "/");
    }
    return node;
}

void DatabaseTool::saveToFile() {
    File file = SPIFFS.open("database.json", "w");
    if (!file) {
        Serial.println("Fehler beim Speichern der JSON-Datei!");
        return;
    }

    serializeJson(jsonDoc, file);
    file.close();
}
