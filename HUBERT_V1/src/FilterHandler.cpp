#include "FilterHandler.h"
#include <Arduino.h> // Für Zeitmessung und Debugging

FilterHandler::FilterHandler(float lowPassCutoff, float highPassCutoff, int movingAvgWindowSize,
                             float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate,
                             PID& rollPID, PID& pitchPID, float sampleRate)
    : lowPassFilter(lowPassCutoff, sampleRate),
      highPassFilter(highPassCutoff, sampleRate),
      movingAvgFilter(movingAvgWindowSize),
      kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      rollPID(rollPID),
      pitchPID(pitchPID),
      cgOffsetX(0), cgOffsetY(0), cgOffsetZ(0),
      lastUpdateTime(0),
      alpha(0.9) {}

void FilterHandler::setCGOffsets(float offsetX, float offsetY, float offsetZ) {
    cgOffsetX = offsetX;
    cgOffsetY = offsetY;
    cgOffsetZ = offsetZ;
}

float FilterHandler::calculateDeltaTime() {
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdateTime) / 1000.0; // Millisekunden in Sekunden umrechnen
    lastUpdateTime = currentTime;

    if (dt <= 0 || dt > 1.0) { // Fallback für ungültige dt-Werte
        dt = 0.01;
    }
    return dt;
}

float FilterHandler::processRoll(float accelX, float accelY, float accelZ, float gyroRate, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    float dt = calculateDeltaTime(); // Zeitdifferenz berechnen

    // Roll-Winkel aus Accelerometer berechnen
    float rollAccelAngle = atan2(accelY, accelZ) * (180.0 / PI);

    // Fusion mit Gyro-Daten (z. B. mit einem Complementary Filter)
    float fusedRoll = complementaryFusion(rollAccelAngle, gyroRate, dt, alpha);

    // PID-Regelung anwenden
    float pidOutput = rollPID.compute(0, fusedRoll); // Sollwert ist 0 (stabiler Zustand)

    // Filter anwenden (optional)
    return applyFilters(-pidOutput, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::processPitch(float accelX, float accelY, float accelZ, float gyroRate, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    float dt = calculateDeltaTime(); // Zeitdifferenz berechnen

    // Pitch-Winkel aus Accelerometer berechnen
    float pitchAccelAngle = atan2(accelX, sqrt(accelY * accelY + accelZ * accelZ)) * (180.0 / PI);

    // Fusion mit Gyro-Daten (z. B. mit einem Complementary Filter)
    float fusedPitch = complementaryFusion(pitchAccelAngle, gyroRate, dt, alpha);

    // PID-Regelung anwenden
    float pidOutput = pitchPID.compute(0, fusedPitch); // Sollwert ist 0 (stabiler Zustand)

    // Filter anwenden (optional)
    return applyFilters(-pidOutput, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    if (useLowPass) value = lowPassFilter.apply(value);
    if (useHighPass) value = highPassFilter.apply(value);
    if (useMovingAvg) value = movingAvgFilter.apply(value);
    return value;
}

float FilterHandler::complementaryFusion(float accelAngle, float gyroRate, float dt, float alpha) {
    // Gyro-Daten korrigieren
    float correctedGyroRate = gyroRate - (accelAngle * cgOffsetX); 

    // Fusion von Gyro- und Accelerometer-Daten
    return alpha * (correctedGyroRate * dt) + (1 - alpha) * accelAngle;
}
