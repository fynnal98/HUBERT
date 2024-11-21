#include "DataLogger.h"

DataLogger::DataLogger() {
    // Konstruktor, falls notwendig
}

// Ausgabe sowohl der ungefilterten als auch der gefilterten Gyroskop-Daten (gx, gy, rollFiltered, pitchFiltered)
void DataLogger::logData(float gx, float gy, float rollFiltered, float pitchFiltered) {
    // Serial.print("Raw: ");
    // Serial.print(gx); Serial.print(",");
    // Serial.print(gy);

    // Wenn keine gefilterten Werte vorhanden sind, ersetze sie durch die Rohwerte der Gyrodaten
    if (rollFiltered == -1 && pitchFiltered == -1) {
        rollFiltered = gx;
        pitchFiltered = gy;
    }

    // Serial.print(" | Filtered: ");
    // Serial.print(rollFiltered); Serial.print(",");
    // Serial.println(pitchFiltered);  
}
