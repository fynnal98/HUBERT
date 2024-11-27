#include "MPU6050.h"
#include <Wire.h>

#define MPU6050_ADDRESS 0x68
#define MPU6050_SMPLRT_DIV 0x19

MPU6050::MPU6050() : mpu(), mpuConnected(false) {}

void MPU6050::begin() {
    if (!Wire.begin()) {
        Serial.println("I2C-Bus konnte nicht initialisiert werden.");
        mpuConnected = false;
        return;
    }

    if (!mpu.begin(MPU6050_ADDRESS)) {
        Serial.println("MPU6050 nicht gefunden!");
        mpuConnected = false;
    } else {
        configureMPU6050();
        mpuConnected = true;
        Serial.println("MPU6050 erfolgreich initialisiert.");
    }
}

void MPU6050::configureMPU6050() {
    // Konfigurationscode bleibt unverändert
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1A); // CONFIG-Register
    Wire.write(0x06); // DLPF auf 5 Hz
    Wire.endTransmission();

    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_SMPLRT_DIV); // SMPLRT_DIV-Register
    Wire.write(0x04); // Sample Rate Divider (200 Hz)
    Wire.endTransmission();

    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1B); // GYRO_CONFIG-Register
    Wire.write(0x18); // ±2000°/s
    Wire.endTransmission();

    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1C); // ACCEL_CONFIG-Register
    Wire.write(0x18); // ±16g
    Wire.endTransmission();
}

void MPU6050::checkConnectionAndBlink() {
    if (!mpuConnected) {
        Serial.println("Verbindung zum MPU6050 verloren. Versuche erneut...");
        while (!mpuConnected) {
            delay(500);
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
        mpuConnected = true;

        // Debugging-Ausgabe der Rohdaten
        // Serial.print("Accel X: "); Serial.println(a->acceleration.x);
        // Serial.print("Gyro Y: "); Serial.println(g->gyro.y);
    } else {
        mpuConnected = false;
        Serial.println("MPU6050 Verbindung verloren!");
    }
}


bool MPU6050::isConnected() const {
    return mpuConnected;
}
