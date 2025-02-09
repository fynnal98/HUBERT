#include "ControlManager.h"
#include <Arduino.h>

ControlManager::ControlManager(JsonHandler* db) : m_db(db), sbus(nullptr) {}

void ControlManager::begin() {
    std::string method = m_db->GetValueByKey<std::string>("config/input/method", "sbus");

    if (method == "sbus") {
        initSbus();
    } else {
        Serial.println("Unbekannte Eingabemethode.");
    }
}

void ControlManager::initSbus() {
    int rxPin = m_db->GetValueByKey<int>("config/input/sbus/rxPin", 16);
    int txPin = m_db->GetValueByKey<int>("config/input/sbus/txPin", 12);

    sbus = new bfs::SbusRx(&Serial2, rxPin, txPin, true);
    sbus->Begin();
    Serial.println("SBUS-Input initialisiert.");
}

void ControlManager::update() {
    if (sbus) {
        updateSbusData();
    }
}

void ControlManager::updateSbusData() {
    if (sbus->Read()) {
        auto data = sbus->data();
        for (int i = 0; i < bfs::SbusData::NUM_CH; ++i) {
            channelValues[i] = data.ch[i];
        }
    }
}

int ControlManager::getRawChannelValue(int channel) {
    if (channel >= 0 && channel < bfs::SbusData::NUM_CH) {
        return channelValues[channel];
    }
    return 1024;  // Standardwert
}
