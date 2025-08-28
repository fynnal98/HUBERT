#ifndef MPU6050_H
#define MPU6050_H

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include "DatabaseTool.h"  

class MPU6050 {
public:
    MPU6050();
    void begin(DatabaseTool* db);  
    void configureMPU6050();
    void checkConnectionAndBlink();
    void getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp);
    bool isConnected() const;

private:
    Adafruit_MPU6050 mpu;
    bool mpuConnected;
};

#endif // MPU6050_H
