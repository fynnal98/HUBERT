#include "InputManager.h"
#include "JsonHandler.h"
#include "SBUSHandler.h"
#include "UDPManager.h"
#include <Arduino.h>

InputManager::InputManager(JsonHandler* db, SBUSHandler* sbusHandler, UDPManager* udpManager)
    : m_db(db), m_sbusHandler(sbusHandler), m_udpManager(udpManager), m_channelValues(16, 1024) {}

void InputManager::begin() {
    // Eingabemethode aus der JSON-Konfiguration laden
    m_inputMethod = m_db->GetValueByKey<std::string>("config/input/method", "sbus");
    Serial.printf("InputManager: Eingabemethode ist '%s'\n", m_inputMethod.c_str());

    if (m_inputMethod == "sbus") {
        m_sbusHandler->begin();
        Serial.println("InputManager: SBUS-Input aktiviert.");
    } else if (m_inputMethod == "wifi") {
        Serial.println("InputManager: WLAN-Input aktiviert.");
    } else {
        Serial.println("InputManager: Unbekannte Eingabemethode.");
    }
}

void InputManager::update() {
    if (m_inputMethod == "sbus") {
        m_sbusHandler->update();
        for (int i = 0; i < m_channelValues.size(); ++i) {
            m_channelValues[i] = m_sbusHandler->getChannelValue(i);
        }
    } else if (m_inputMethod == "wifi") {
        for (int i = 0; i < m_channelValues.size(); ++i) {
            m_channelValues[i] = m_udpManager->getRawChannelValue(i);
        }
    }


    // for (int i = 0; i < m_channelValues.size(); ++i) {
    //     Serial.printf("Kanal %d: %d ", i, m_channelValues[i]);
    // }
    // Serial.println();
}

int InputManager::getRawChannelValue(int channel) {
    if (channel >= 0 && channel < m_channelValues.size()) {
        return m_channelValues[channel];
    }
    return 1024;  // Standardwert
}
