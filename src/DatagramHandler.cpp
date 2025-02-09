#include "DatagramHandler.h"
#include "JsonHandler.h"
#include <Arduino.h>

// Externe JSON-Handler-Instanz (wird im Hauptprogramm initialisiert)
extern JsonHandler jsonHandler;

std::vector<uint8_t> DatagramHandler::createDatagram(const std::string& group, const std::string& jsonData) {
    std::vector<uint8_t> datagram;

    // Startbyte
    datagram.push_back(0x20);

    // Finde den Payload-Typ der Gruppe
    std::string payloadTypePath = "payload-type-mapping/" + group;
    uint8_t payloadType = jsonHandler.GetValueByKey<uint8_t>(payloadTypePath, 0xFF);

    datagram.push_back(payloadType);

    // Nachrichtenzähler (Dummy-Wert hier)
    static uint8_t msgCounter = 0;
    datagram.push_back(msgCounter++);

    // JSON-Payload in Bytes konvertieren
    std::vector<uint8_t> payload(jsonData.begin(), jsonData.end());
    datagram.push_back(payload.size());

    // Payload-Daten anhängen
    datagram.insert(datagram.end(), payload.begin(), payload.end());

    // Checksumme berechnen
    uint8_t checksum = calculateChecksum(datagram);
    datagram.push_back(checksum);

    // Endbyte
    datagram.push_back(0x30);

    return datagram;
}

void DatagramHandler::parseDatagram(const std::vector<uint8_t>& datagram) {
    if (datagram.size() < 6) {
        Serial.println("Ungültige Datagrammlänge.");
        return;
    }

    // Start- und Endbyte prüfen
    if (datagram[0] != 0x20 || datagram.back() != 0x30) {
        Serial.println("Ungültiges Start- oder Endbyte.");
        return;
    }

    // Checksumme prüfen
    uint8_t receivedChecksum = datagram[datagram.size() - 2];
    if (receivedChecksum != calculateChecksum(std::vector<uint8_t>(datagram.begin(), datagram.end() - 2))) {
        Serial.println("Checksummenfehler.");
        return;
    }

    uint8_t payloadType = datagram[1];
    uint8_t payloadSize = datagram[3];

    // Prüfen, ob die angegebene Payload-Größe stimmt
    if (payloadSize != (datagram.size() - 6)) {
        Serial.println("Fehlerhafte Payload-Größe.");
        return;
    }

    std::vector<uint8_t> payload(datagram.begin() + 4, datagram.begin() + 4 + payloadSize);

    // Finde die passende Gruppe anhand des Payload-Typs
    std::string groupKey = findGroupByPayloadType(payloadType);
    if (groupKey.empty()) {
        Serial.println("Keine passende Gruppe für den Payload-Typ gefunden.");
        return;
    }

    Serial.print("Verarbeite Nachricht für Gruppe: ");
    Serial.println(groupKey.c_str());

    // Payload dekodieren
    decodePayload(groupKey, payload);
}

std::string DatagramHandler::findGroupByPayloadType(uint8_t payloadType) {
    JsonVariant mapping = jsonHandler.resolvePath("payload-type-mapping");
    for (JsonPair kv : mapping.as<JsonObject>()) {
        uint8_t type = strtol(kv.key().c_str(), nullptr, 16);
        if (type == payloadType) {
            return kv.value().as<std::string>();
        }
    }
    return "";
}

void DatagramHandler::decodePayload(const std::string& groupKey, const std::vector<uint8_t>& payload) {
    JsonVariant groupData = jsonHandler.resolvePath(groupKey);
    if (groupData.isNull()) {
        Serial.println("Keine gültige Struktur zum Dekodieren gefunden.");
        return;
    }

    // JSON-Dokument aus dem Payload erstellen
    StaticJsonDocument<256> doc;
    DeserializationError error = deserializeJson(doc, std::string(payload.begin(), payload.end()));

    if (error) {
        Serial.print("Fehler beim Dekodieren des Payloads: ");
        Serial.println(error.c_str());
        return;
    }

    Serial.print("Dekodierte Daten für Gruppe ");
    Serial.println(groupKey.c_str());

    // Alle Werte der Gruppe ausgeben
    for (JsonPair kv : groupData.as<JsonObject>()) {
        const char* key = kv.key().c_str();
        if (doc[key].is<float>()) {
            Serial.print(key);
            Serial.print(": ");
            Serial.println(doc[key].as<float>(), 6);
        } else if (doc[key].is<int>()) {
            Serial.print(key);
            Serial.print(": ");
            Serial.println(doc[key].as<int>());
        } else if (doc[key].is<bool>()) {
            Serial.print(key);
            Serial.print(": ");
            Serial.println(doc[key].as<bool>() ? "true" : "false");
        } else if (doc[key].is<const char*>()) {
            Serial.print(key);
            Serial.print(": ");
            Serial.println(doc[key].as<const char*>());
        } else {
            Serial.print(key);
            Serial.println(": Unbekannter Typ");
        }
    }
}

uint8_t DatagramHandler::calculateChecksum(const std::vector<uint8_t>& data) {
    uint8_t checksum = 0;
    for (const auto& byte : data) {
        checksum += byte;
    }
    return checksum;
}
