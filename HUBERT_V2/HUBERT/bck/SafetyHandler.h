#ifndef SENSORHANDLER_H
#define SENSORHANDLER_H

#include "MPU6050.h"
#include "DatabaseTool.h"

class SensorHandler {
public:
    void init(DatabaseTool* db);
    void update();

    float getRawAccelX() { return accelX; }
    float getRawAccelY() { return accelY; }
    float getRawAccelZ() { return accelZ; }

    float getRawGyroX() { return gyroX; }
    float getRawGyroY() { return gyroY; }
    float getRawGyroZ() { return gyroZ; }

private:
    MPU6050 mpu;
    float accelX, accelY, accelZ;
    float gyroX, gyroY, gyroZ;
};

#endif
