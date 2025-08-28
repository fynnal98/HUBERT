#ifndef SBUSHANDLER_H
#define SBUSHANDLER_H

#include <string>
#include <vector>
#include "sbus.h"  // Korrekte SBUS-Bibliothek

class SBUSHandler {
public:
    SBUSHandler();  // Standardkonstruktor

    void begin();  // Initialisiert die SBUS-Verbindung
    void update();  // Liest neue SBUS-Daten
    std::string getInput();  // Gibt die Daten als String aus
    int getChannelValue(int channel);  // Gibt den Wert eines Kanals zurück

private:
    bfs::SbusRx* sbus;  // Verwende die korrekte Klasse bfs::SbusRx
    std::vector<int> channelData;
    std::string receivedData;

    int rxPin;
    int txPin;

    bool loadConfig();
};

#endif
