#include "MPU6050.h"
#include "DataLogger.h"

#define MPU6050_ADDRESS 0x68  
#define MPU6050_SMPLRT_DIV 0x19  

MPU6050::MPU6050() : mpu(), cgOffsetX(0.0), cgOffsetY(0.0), cgOffsetZ(0.0), mpuConnected(true) {}  // Initialisiere mpuConnected

void MPU6050::begin() {
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        mpuConnected = false;  // Setze mpuConnected auf false, wenn die Initialisierung fehlschlägt
        while (1) {
            delay(10);
        }
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
    mpu.setGyroRange(MPU6050_RANGE_1000_DEG);
    mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
    
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_SMPLRT_DIV);
    Wire.write(0x00); 
    Wire.endTransmission();
    
    mpuConnected = true;  // Initialisiere mpuConnected auf true, wenn alles funktioniert
}

void MPU6050::setup() {}

void MPU6050::getEvent(sensors_event_t* a, sensors_event_t* g, sensors_event_t* temp) {
    if (mpu.getEvent(a, g, temp)) {
        mpuConnected = true;  // Wenn der Sensor reagiert, ist die Verbindung vorhanden
        DataLogger dataLogger;
        dataLogger.logData(g->gyro.x, g->gyro.y);
    } else {
        mpuConnected = false;  // Wenn der Sensor nicht reagiert, setze mpuConnected auf false
    }
}

void MPU6050::applyGyroOffset(sensors_event_t& g, float gyroXOffset, float gyroYOffset, float gyroZOffset) {
    g.gyro.x -= gyroXOffset;
    g.gyro.y -= gyroYOffset;
    g.gyro.z -= gyroZOffset;
}

void MPU6050::calculateCorrectedAccelerations(sensors_event_t* a, sensors_event_t* g, float& ax_corrected, float& ay_corrected) {
    ax_corrected = a->acceleration.x - (g->gyro.y * cgOffsetZ - g->gyro.z * cgOffsetY);
    ay_corrected = a->acceleration.y - (g->gyro.z * cgOffsetX - g->gyro.x * cgOffsetZ);
}

void MPU6050::setCGOffsets(float offsetX, float offsetY, float offsetZ) {
    this->cgOffsetX = offsetX;
    this->cgOffsetY = offsetY;
    this->cgOffsetZ = offsetZ;
}

// Neue Methode, um den Verbindungsstatus des MPU zu überprüfen
bool MPU6050::isConnected() const {
    return mpuConnected;
}
