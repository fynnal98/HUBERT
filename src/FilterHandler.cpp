#include "FilterHandler.h"
#include <Arduino.h> // Für Serial-Debugging

FilterHandler::FilterHandler(float lowPassCutoff, float highPassCutoff, int movingAvgWindowSize,
                             float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, float sampleRate)
    : lowPassFilter(lowPassCutoff, sampleRate),
      highPassFilter(highPassCutoff, sampleRate),
      movingAvgFilter(movingAvgWindowSize),
      kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      cgOffsetX(0), cgOffsetY(0), cgOffsetZ(0),
      integratedGyroRoll(0), integratedGyroPitch(0) {}


void FilterHandler::setCGOffsets(float offsetX, float offsetY, float offsetZ) {
    cgOffsetX = offsetX;
    cgOffsetY = offsetY;
    cgOffsetZ = offsetZ;

    // Debugging-Ausgabe
    Serial.print("Set CG Offsets - X: ");
    Serial.print(cgOffsetX);
    Serial.print(", Y: ");
    Serial.print(cgOffsetY);
    Serial.print(", Z: ");
    Serial.println(cgOffsetZ);
}

float FilterHandler::processRoll(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    // Debugging-Ausgabe der Eingangswerte
    Serial.println("\n[Processing Roll]");
    Serial.print("Input Accel: ");
    Serial.println(accel);
    Serial.print("Input Gyro: ");
    Serial.println(gyro);
    Serial.print("dt: ");
    Serial.println(dt);

    // Zeitschritt absichern
    if (dt <= 0 || dt > 1.0) {
        dt = 0.01; // Standardwert
        Serial.println("Warning: dt reset to 0.01");
    }

    // Korrigiere die Beschleunigungsdaten
    float correctedAccel = accel - (gyro * cgOffsetZ);

    // Debugging-Ausgabe der korrigierten Beschleunigungsdaten
    Serial.print("Corrected Accel: ");
    Serial.println(correctedAccel);

    // Integriere Gyro-Daten
    integratedGyroRoll += gyro * dt;
    Serial.print("Integrated Gyro Roll: ");
    Serial.println(integratedGyroRoll);

    // Führe die Fusion durch
    float fusedValue = kalmanFilter.update(integratedGyroRoll, correctedAccel, dt);

    // Debugging-Ausgabe des Fusionsergebnisses
    Serial.print("Fused Roll Value: ");
    Serial.println(fusedValue);

    // Anwenden der Filter
    return applyFilters(fusedValue, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::processPitch(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    // Debugging-Ausgabe der Eingangswerte
    Serial.println("\n[Processing Pitch]");
    Serial.print("Input Accel: ");
    Serial.println(accel);
    Serial.print("Input Gyro: ");
    Serial.println(gyro);
    Serial.print("dt: ");
    Serial.println(dt);

    // Zeitschritt absichern
    if (dt <= 0 || dt > 1.0) {
        dt = 0.01; // Standardwert
        Serial.println("Warning: dt reset to 0.01");
    }

    // Korrigiere die Beschleunigungsdaten
    float correctedAccel = accel - (gyro * cgOffsetX);

    // Debugging-Ausgabe der korrigierten Beschleunigungsdaten
    Serial.print("Corrected Accel: ");
    Serial.println(correctedAccel);

    // Integriere Gyro-Daten
    integratedGyroPitch += gyro * dt;
    Serial.print("Integrated Gyro Pitch: ");
    Serial.println(integratedGyroPitch);

    // Führe die Fusion durch
    float fusedValue = kalmanFilter.update(integratedGyroPitch, correctedAccel, dt);

    // Debugging-Ausgabe des Fusionsergebnisses
    Serial.print("Fused Pitch Value: ");
    Serial.println(fusedValue);

    // Anwenden der Filter
    return applyFilters(fusedValue, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    if (useLowPass) {
        value = lowPassFilter.apply(value);
        Serial.print("After LowPass Filter: ");
        Serial.println(value);
    }
    if (useHighPass) {
        value = highPassFilter.apply(value);
        Serial.print("After HighPass Filter: ");
        Serial.println(value);
    }
    if (useMovingAvg) {
        value = movingAvgFilter.apply(value);
        Serial.print("After MovingAvg Filter: ");
        Serial.println(value);
    }
    return value;
}
