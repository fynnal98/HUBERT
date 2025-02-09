#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_MPU6050.h>
#include <Wire.h>
#include "JsonHandler.h"

class MPU6050 {
public:
    MPU6050();
    void begin(JsonHandler* db);  // Initialisiert die Verbindung zum MPU6050
    bool isConnected() const;  // Prüft, ob der Sensor verbunden ist
    void checkConnectionAndBlink();  // Prüft die Verbindung und versucht sie wiederherzustellen
    void getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);  // Holt die Sensordaten

private:
    Adafruit_MPU6050 mpu;
    bool mpuConnected;
    void configureMPU6050();  // Konfiguriert den Sensor (DLPF, Gyro-, Accel-Empfindlichkeit)
};

#endif
