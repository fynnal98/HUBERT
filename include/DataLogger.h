#ifndef DATALOGGER_H
#define DATALOGGER_H

#include <Arduino.h>

class DataLogger {
public:
    DataLogger();
    // Protokolliert die Gyroskopdaten gx, gy und optional gefilterte Roll- und Pitch-Werte
    void logData(float gx, float gy, float rollFiltered = -1, float pitchFiltered = -1);
};

#endif // DATALOGGER_H
