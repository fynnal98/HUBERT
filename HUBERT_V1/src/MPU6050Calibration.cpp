#include "MPU6050Calibration.h"
#include "ParameterHandler.h"

extern int ledPin;  // Verweis auf die globale Variable in ParameterHandler.cpp

int MPU6050Calibration::calibrationStep = 0;
int MPU6050Calibration::totalSamples = 0;
float MPU6050Calibration::gyroXSum = 0;
float MPU6050Calibration::gyroYSum = 0;
float MPU6050Calibration::gyroZSum = 0;

void MPU6050Calibration::beginCalibration(int numSamples) {
    calibrationStep = 0;
    totalSamples = numSamples;
    gyroXSum = 0;
    gyroYSum = 0;

    // Initialisiere die LED als Ausgang
    pinMode(ledPin, OUTPUT);
}

bool MPU6050Calibration::updateCalibration(MPU6050& mpu, float& gyroXOffset, float& gyroYOffset, float& gyroZOffset) {
    if (calibrationStep >= totalSamples) {
        // Berechne die Offsets
        gyroXOffset = gyroXSum / totalSamples;
        gyroYOffset = gyroYSum / totalSamples;
        gyroZOffset = gyroZSum / totalSamples;

        Serial.println("Kalibrierung abgeschlossen");

        // LED zweimal blinken lassen
        for (int i = 0; i < 2; i++) {
            digitalWrite(ledPin, HIGH);
            delay(200);
            digitalWrite(ledPin, LOW);
            delay(200);
        }

        return true;
    }

    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    gyroXSum += g.gyro.x;
    gyroYSum += g.gyro.y;
    gyroZSum += g.gyro.z;

    calibrationStep++;

    if (calibrationStep % 100 == 0) {
        Serial.print("Kalibrierung Schritt: ");
        Serial.println(calibrationStep);
    }

    return false;
}

void MPU6050Calibration::resetCalibration() {
    calibrationStep = 0;
    gyroXSum = 0;
    gyroYSum = 0;
    gyroZSum = 0;
}
