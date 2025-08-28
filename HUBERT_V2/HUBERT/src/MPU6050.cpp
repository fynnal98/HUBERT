#include "MPU6050.h"
#include <Wire.h>

#define MPU6050_ADDRESS 0x68

MPU6050::MPU6050() : mpu(), mpuConnected(false) {}

void MPU6050::begin(DatabaseTool* db) {
    // Pins aus der Datenbank lesen
    int sda = db->get<int>("sensors/mpu6050/sda", 8); 
    int scl = db->get<int>("sensors/mpu6050/scl", 9);  

    // I²C mit den definierten Pins starten
    Wire.begin(sda, scl);

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
    Wire.beginTransmission(MPU6050_ADDRESS);

    // CONFIG (Digital Low Pass Filter auf 5 Hz, maximale Filterung)
    Wire.write(0x1A);
    Wire.write(0x06);
    Wire.endTransmission();

    // GYRO_CONFIG (±250°/s)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1B);
    Wire.write(0x00);
    Wire.endTransmission();

    // ACCEL_CONFIG (±2g)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1C);
    Wire.write(0x00);
    Wire.endTransmission();
}

bool MPU6050::isConnected() const {
    return mpuConnected;
}

void MPU6050::checkConnectionAndBlink() {
    if (!mpuConnected) {
        Serial.println("Verbindung zum MPU6050 verloren. Versuche erneut...");
        while (!mpuConnected) {
            delay(100);
            mpuConnected = mpu.begin(MPU6050_ADDRESS);
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
        // Serial.print("Accelerometer: ");
        // Serial.print("X: "); Serial.print(a->acceleration.x); Serial.print(" ");
        // Serial.print("Y: "); Serial.print(a->acceleration.y); Serial.print(" ");
        // Serial.print("Z: "); Serial.println(a->acceleration.z);
    } else {
        mpuConnected = false;
        Serial.println("Fehler beim Lesen der MPU6050-Daten.");
    }
}