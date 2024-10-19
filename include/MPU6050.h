#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

class MPU6050 {
public:
    MPU6050();
    void begin();
    void setup();
    void getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);
    void applyGyroOffset(sensors_event_t& g, float gyroXOffset, float gyroYOffset, float gyroZOffset);
    void calculateCorrectedAccelerations(sensors_event_t* a, sensors_event_t* g, float& ax_corrected, float& ay_corrected);
    void setCGOffsets(float offsetX, float offsetY, float offsetZ);
    
    // Neue Methode, um den Verbindungsstatus des MPU zu überprüfen
    bool isConnected() const;

private:
    Adafruit_MPU6050 mpu;
    float cgOffsetX, cgOffsetY, cgOffsetZ;

    // Variable, um den Verbindungsstatus des MPU zu speichern
    bool mpuConnected;
};

#endif // MPU6050_H
