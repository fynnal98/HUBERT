#include "InputHandler.h"
#include <sstream>
#include <Arduino.h>

InputHandler* InputHandler::createHandler(DatabaseTool* db) {
    InputHandler* handler = new InputHandler();
    handler->init(db);
    return handler;
}

void InputHandler::init(DatabaseTool* db) {
    std::string method = db->get<std::string>("input/method", "sbus");

    if (method == "wifi") {
        // WiFiInput initialisieren
        const char* ssid = db->get<const char*>("Wifi/ssid", "HUBERT");
        const char* password = db->get<const char*>("Wifi/password", "12345678");
        int port = db->get<int>("Wifi/port", 4210);

        wifiInput = new WiFiInput();
        wifiInput->init(db, this);
        Serial.println("WiFiInput für App-Steuerung aktiv.");
    } 
    else if (method == "sbus") {
        int rxPin = db->get<int>("input/sbus/rxPin", 16);
        int txPin = db->get<int>("input/sbus/txPin", 17);
        sbusInput = new SBUSInput(&Serial1, rxPin, txPin);
        sbusInput->init(db);
        Serial.println("SBUS-Steuerung aktiv.");
    }

    std::string mode = db->get<std::string>("mode/type", "drone");
    std::string channelPath = "mode/" + mode + "/Channels";
    JsonVariant channels = db->resolvePath(channelPath.c_str());

    if (channels.is<JsonObject>()) {
        for (JsonPair kv : channels.as<JsonObject>()) {
            channelMapping[kv.key().c_str()] = kv.value().as<int>();
        }
    } else {
        Serial.println("Kanalzuordnung nicht gefunden oder ungültig!");
    }

    for (const auto& pair : channelMapping) {
        channelValues[pair.first] = 1024;
    }
}

void InputHandler::update() {
    std::string rawData;

    if (wifiInput) {
        wifiInput->update();
        rawData = wifiInput->getInput();
    }

    if (sbusInput) {
        sbusInput->update();
        rawData = sbusInput->getInput();
    }

    if (!rawData.empty()) {
        updateWithAppData(rawData);
    }
}

void InputHandler::updateWithAppData(const std::string& rawData) {
    if (!rawData.empty()) {
        std::vector<int> parsedValues;
        std::istringstream ss(rawData);
        std::string token;

        while (std::getline(ss, token, ',')) {
            parsedValues.push_back(std::stoi(token));
        }

        for (const auto& pair : channelMapping) {
            int index = pair.second - 1;
            if (index >= 0 && index < parsedValues.size()) {
                channelValues[pair.first] = parsedValues[index];
                // Serial.printf("Kanal: %s, Wert: %d\n", pair.first.c_str(), channelValues[pair.first]);
            }
        }
    }
}

std::string InputHandler::getInput() {
    if (wifiInput) {
        return wifiInput->getInput();
    }
    if (sbusInput) {
        return sbusInput->getInput();
    }
    return "";
}

int InputHandler::getChannelValue(const std::string& channel) {
    if (channelValues.find(channel) != channelValues.end()) {
        return channelValues[channel];
    }
    return 1024;
}
