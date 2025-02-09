#ifndef CONTROLHANDLER_H
#define CONTROLHANDLER_H

#include <map>
#include <string>
#include "sbus.h"
#include "JsonHandler.h"

class ControlManager {
public:
    ControlManager(JsonHandler* db);
    void begin();  // Initialisiert den ControlManager
    void update();  // Liest die aktuellen Steuerwerte aus
    int getRawChannelValue(int channel);  // Liefert den Wert eines SBUS-Kanals

private:
    JsonHandler* m_db;
    bfs::SbusRx* sbus;  // SBUS-Eingang
    int channelValues[bfs::SbusData::NUM_CH];  // Array für die Rohdaten

    void initSbus();  // Initialisiert den SBUS
    void updateSbusData();  // SBUS-Daten aktualisieren
};

#endif
