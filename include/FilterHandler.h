#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H

#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"
#include "PID.h"

class FilterHandler {
public:
    // Konstruktor
    FilterHandler(float lowPassCutoff, float highPassCutoff, int movingAvgWindowSize,
                  float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate,
                  PID& rollPID, PID& pitchPID, float sampleRate);

    // Methoden
    void setCGOffsets(float offsetX, float offsetY, float offsetZ);
    float processRoll(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg);
    float processPitch(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg);

private:
    // Filterinstanzen
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;

    // PID-Referenzen
    PID& rollPID;
    PID& pitchPID;

    // Schwerpunkt-Offsets
    float cgOffsetX, cgOffsetY, cgOffsetZ;

    // Integrierte Gyrodaten
    float integratedGyroRoll;
    float integratedGyroPitch;

    // Methode zur Anwendung von Filtern
    float applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg);
};

#endif // FILTERHANDLER_H
