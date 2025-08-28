#ifndef SBUSINPUT_H
#define SBUSINPUT_H

#include "DatabaseTool.h"
#include "sbus.h"  // Einbinden der SBUS-Bibliothek
#include <string>

class SBUSInput {
public:
    SBUSInput(HardwareSerial* serial, int rxPin, int txPin);
    void init(DatabaseTool* db);  // Initialisierungsmethode
    void update();  // Update-Methode, um die Daten einzulesen
    std::string getInput();  // Gibt die empfangenen Daten als String zurück

private:
    bfs::SbusRx sbus;  // SBUS-Objekt
    std::string receivedData;  // Puffer für empfangene Daten
};

#endif // SBUSINPUT_H
