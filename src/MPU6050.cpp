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
    Wire.beginTransmission(MPU6050_ADDRESS);

    // CONFIG (Digital Low Pass Filter auf 5 Hz, maximale Filterung)
    Wire.write(0x1A); 
    Wire.write(0x06); 
    Wire.endTransmission();

    // SMPLRT_DIV (Abtastrate auf 250 Hz einstellen)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(MPU6050_SMPLRT_DIV); 
    Wire.write(0x03); 
    Wire.endTransmission();

    // GYRO_CONFIG (±250°/s, geringste Empfindlichkeit für Gyroskop)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1B); 
    Wire.write(0x00); 
    Wire.endTransmission();

    // ACCEL_CONFIG (±2g, geringste Empfindlichkeit für Beschleunigungsmesser)
    Wire.beginTransmission(MPU6050_ADDRESS);
    Wire.write(0x1C); 
    Wire.write(0x00); 
    Wire.endTransmission();
}



void MPU6050::checkConnectionAndBlink() {
    if (!mpuConnected) {
        Serial.println("Verbindung zum MPU6050 verloren. Versuche erneut...");
        while (!mpuConnected) {
            delay(20);
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
