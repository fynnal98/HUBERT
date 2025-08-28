#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "JsonHandler.h"
#include "Adafruit_MPU6050.h"
#include <math.h>

class SensorHandler {
public:
    SensorHandler();
    void begin(JsonHandler* db);  // Initialisierung der Sensoren und Filterparameter
    void update();  // Sensorwerte erfassen und filtern
    void getFilteredData(float& roll, float& pitch, float& yaw);

private:
    Adafruit_MPU6050 mpu6050;

    float filteredRoll, filteredPitch, filteredYaw;
    bool complementaryEnabled;
    float alpha;
    bool lowPassEnabled;
    float lowPassCutoff;
    bool highPassEnabled;
    float highPassCutoff;

    void applyComplementaryFilter(float& roll, float& pitch, float& yaw, const sensors_event_t& accel, const sensors_event_t& gyro, float dt);
    void applyLowPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt);
    void applyHighPassFilter(float& value, float& previousValue, float cutoffFrequency, float dt);
};

#endif
