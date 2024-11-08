#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "ParameterHandler.h"  // Für Zugriff auf den LED-Pin

class MPU6050 {
public:
    MPU6050();
    void begin();
    void getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);
    void applyGyroOffset(sensors_event_t& g, float gyroXOffset, float gyroYOffset, float gyroZOffset);
    void calculateCorrectedAccelerations(sensors_event_t* a, sensors_event_t* g, float& ax_corrected, float& ay_corrected);
    void setCGOffsets(float offsetX, float offsetY, float offsetZ);

    // Methode zur Überprüfung des Verbindungsstatus und zum Blinken der LED bei Abbruch
    void checkConnectionAndBlink();
    
    bool isConnected() const;

private:
    Adafruit_MPU6050 mpu;
    float cgOffsetX, cgOffsetY, cgOffsetZ;
    bool mpuConnected;  // Speichert den Verbindungsstatus zum MPU6050
};

#endif // MPU6050_H
