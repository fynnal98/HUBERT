#ifndef PLANELOGIC_H
#define PLANELOGIC_H

#include "SensorHandler.h"
#include "InputHandler.h"

class PlaneLogic {
public:
    void init(DatabaseTool* db) {
        Serial.println("Flugzeugmodus initialisiert.");
    }

    void update(SensorHandler* sensors, InputHandler* input) {
        Serial.println("Flugzeugmodus wird aktualisiert.");
    }
};

#endif
