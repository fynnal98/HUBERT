#ifndef FILTERHANDLER_H
#define FILTERHANDLER_H

#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"

class FilterHandler {
public:
    FilterHandler(float lowPassCutoff, float highPassCutoff, int movingAvgWindowSize,
                  float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, float sampleRate);

    void setCGOffsets(float offsetX, float offsetY, float offsetZ);

    // Prozessierungsfunktionen für Roll und Pitch
    float processRoll(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg);
    float processPitch(float accel, float gyro, float dt, bool useLowPass, bool useHighPass, bool useMovingAvg);

private:
    // Hilfsfunktionen
    float applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg);
    float integratedGyroRoll;  // Integrierter Gyro-Wert für Roll
    float integratedGyroPitch;
    // Filterinstanzen
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;

    // Schwerpunkt-Offsets
    float cgOffsetX, cgOffsetY, cgOffsetZ;

    // Abtastrate
    float sampleRate;
};

#endif // FILTERHANDLER_H
