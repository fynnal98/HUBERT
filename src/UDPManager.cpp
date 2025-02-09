#include "UDPManager.h"
#include <Arduino.h>

UDPManager::UDPManager() : m_channelValues(10, 1024) {}  // 10 Kanäle mit Standardwert 1024 initialisieren

void UDPManager::handleMessage(uint8_t type, const std::vector<uint8_t>& payload) {
    if (type == 0x10) {
        updateChannelValues(payload);  // Steuerdaten aktualisieren
        Serial.println("UDPManager: Kanalwerte aktualisiert.");
    } else {
        Serial.printf("UDPManager: Unbekannter Nachrichtentyp: 0x%02X\n", type);
    }
}

void UDPManager::updateChannelValues(const std::vector<uint8_t>& payload) {
    for (size_t i = 0; i < m_channelValues.size() && (i * 2 + 1) < payload.size(); ++i) {
        m_channelValues[i] = (payload[i * 2] << 8) | payload[i * 2 + 1];  // 16-Bit-Wert zusammensetzen
    }
}

int UDPManager::getRawChannelValue(int channel) {
    if (channel >= 0 && channel < m_channelValues.size()) {
        return m_channelValues[channel];
    }
    return 1024;  // Standardwert
}
