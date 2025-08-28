#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "DatabaseTool.h"
#include "MPU6050.h"


class SensorHandler {
public:
    SensorHandler();
    void begin(DatabaseTool* db);  // Initialisiert den Sensor und Filter
    void update();  // Liest die MPU6050-Daten und wendet Filter an
    void getFilteredData(float& roll, float& pitch, float& yaw);  // Gibt gefilterte Werte zurück

private:
    MPU6050 mpu6050;

    // Gefilterte Werte
    float filteredRoll;
    float filteredPitch;
    float filteredYaw;

    // Filterparameter
    bool complementaryEnabled;
    float alpha;  // Komplementärfilter Glättungsfaktor

    bool lowPassEnabled;
    float lowPassCutoff;

    bool highPassEnabled;
    float highPassCutoff;

    // Hilfsmethoden für Filter
    void applyComplementaryFilter(float& roll, float& pitch, float& yaw, const sensors_event_t& accel, const sensors_event_t& gyro, float dt);
    void applyLowPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt);
    void applyHighPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt);
};

#endif
