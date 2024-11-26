#include <Wire.h>
#include "MPU6050.h"
#include "DataLogger.h"
#include "ParameterHandler.h"  // Um auf den LED-Pin zuzugreifen

#define MPU6050_ADDRESS 0x68  
#define MPU6050_SMPLRT_DIV 0x19

extern int ledPin;

MPU6050::MPU6050() : mpu(), cgOffsetX(0.0), cgOffsetY(0.0), cgOffsetZ(0.0), mpuConnected(true) {}

void MPU6050::begin() {
    Wire.begin();

    // Versuche, den Sensor zu starten
    if (!mpu.begin()) {
        Serial.println("Failed to find MPU6050 chip");
        mpuConnected = false;  // Setze mpuConnected auf false, wenn die Initialisierung fehlschlägt
    } else {
        configureMPU6050();
        mpuConnected = true;
    }

    pinMode(ledPin, OUTPUT);  // Initialisiere den LED-Pin
}

void MPU6050::configureMPU6050() {
    Wire.beginTransmission(MPU6050_ADDRESS);

    // DLPF auf 5 Hz setzen
    Wire.write(0x1A); // CONFIG-Register
    Wire.write(0x06); // DLPF auf 5 Hz
    Wire.endTransmission();

    // Abtastrate auf 200 Hz reduzieren
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_SMPLRT_DIV); // SMPLRT_DIV-Register
    Wire.write(0x04); // Sample Rate Divider (200 Hz)
    Wire.endTransmission();

    // Gyro-Empfindlichkeit auf ±2000°/s setzen
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1B); // GYRO_CONFIG-Register
    Wire.write(0x18); // ±2000°/s
    Wire.endTransmission();

    // Accelerometer-Empfindlichkeit auf ±16g setzen
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1C); // ACCEL_CONFIG-Register
    Wire.write(0x18); // ±16g
    Wire.endTransmission();
}

void MPU6050::checkConnectionAndBlink() {
    if (!mpuConnected) {
        Serial.println("Verbindung zum MPU6050 verloren. Versuche erneut...");
        while (!mpuConnected) {
            digitalWrite(ledPin, HIGH);
            delay(500);
            digitalWrite(ledPin, LOW);
            delay(500);

            // Versuche, die Verbindung wiederherzustellen
            mpuConnected = mpu.begin();
            if (mpuConnected) {
                configureMPU6050();
                Serial.println("Verbindung zum MPU6050 wiederhergestellt.");
            }
        }
    }
}

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

bool MPU6050::isConnected() const {
    return mpuConnected;
}
