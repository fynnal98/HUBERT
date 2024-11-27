#include "FilterHandler.h"
#include <Arduino.h> // Für Debugging mit Serial

FilterHandler::FilterHandler(float lowPassCutoff, float highPassCutoff, int movingAvgWindowSize,
                             float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate,
                             PID& rollPID, PID& pitchPID, float sampleRate)
    : lowPassFilter(lowPassCutoff, sampleRate),
      highPassFilter(highPassCutoff, sampleRate),
      movingAvgFilter(movingAvgWindowSize),
      kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      rollPID(rollPID), // Initialisierung der Referenzen
      pitchPID(pitchPID),
      cgOffsetX(0), cgOffsetY(0), cgOffsetZ(0),
      integratedGyroRoll(0), integratedGyroPitch(0) {}




void FilterHandler::setCGOffsets(float offsetX, float offsetY, float offsetZ) {
    cgOffsetX = offsetX;
    cgOffsetY = offsetY;
    cgOffsetZ = offsetZ;
}

float FilterHandler::processRoll(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    // Absicherung des Zeitschritts
    if (dt <= 0 || dt > 1.0) dt = 0.01;

    // Korrigiere die Beschleunigungsdaten
    float correctedAccel = accel - (gyro * cgOffsetZ);

    // // Integriere Gyro-Daten
    // integratedGyroRoll += gyro * dt;

    // // Fusion: Kalman-Filter kombiniert Gyro- und Beschleunigungsdaten
    // float fusedValue = kalmanFilter.update(integratedGyroRoll, correctedAccel, dt);

    // // PID-Regelung für die Roll-Achse
    // float pidOutput = rollPID.compute(fusedValue, 0); // Sollwert ist 0 (stabil)

    // Filter anwenden
    return applyFilters(correctedAccel, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::processPitch(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    // Absicherung des Zeitschritts
    if (dt <= 0 || dt > 1.0) dt = 0.01;

    // Korrigiere die Beschleunigungsdaten
    float correctedAccel = accel - (gyro * cgOffsetX);

    // // Integriere Gyro-Daten
    // integratedGyroPitch += gyro * dt;

    // // Fusion: Kalman-Filter kombiniert Gyro- und Beschleunigungsdaten
    // float fusedValue = kalmanFilter.update(integratedGyroPitch, correctedAccel, dt);

    // // PID-Regelung für die Pitch-Achse
    // float pidOutput = pitchPID.compute(fusedValue, 0); // Sollwert ist 0 (stabil)

    // Filter anwenden
    return applyFilters(correctedAccel, useLowPass, useHighPass, useMovingAvg);
}

float FilterHandler::applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg) {
    if (useLowPass) value = lowPassFilter.apply(value);
    if (useHighPass) value = highPassFilter.apply(value);
    if (useMovingAvg) value = movingAvgFilter.apply(value);
    return value;
}
