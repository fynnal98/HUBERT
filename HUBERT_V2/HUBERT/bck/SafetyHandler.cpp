#include "SensorHandler.h"

void SensorHandler::init(DatabaseTool* db) {
    mpu.init();
}

void SensorHandler::update() {
    // Beschleunigungs- und Gyroskopdaten aktualisieren
    mpu.readAccel(accelX, accelY, accelZ);
    mpu.readGyro(gyroX, gyroY, gyroZ);
}
