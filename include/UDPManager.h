#ifndef UDP_MANAGER_H
#define UDP_MANAGER_H

#include <vector>
#include <stdint.h>

class UDPManager {
public:
    UDPManager();
    void handleMessage(uint8_t type, const std::vector<uint8_t>& payload);
    int getRawChannelValue(int channel);  // Holt den aktuellen Wert eines Kanals

private:
    std::vector<int> m_channelValues;  // Speichert die Kanalwerte
    void updateChannelValues(const std::vector<uint8_t>& payload);  // Aktualisiert die Kanalwerte basierend auf der Payload
};

#endif // UDP_MANAGER_H
