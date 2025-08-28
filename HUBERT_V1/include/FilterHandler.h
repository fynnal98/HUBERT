#ifndef FILTER_HANDLER_H
#define FILTER_HANDLER_H

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

    // Setzt die Schwerpunkt-Offsets
    void setCGOffsets(float offsetX, float offsetY, float offsetZ);

    // Verarbeitung für Roll
    float processRoll(float accelX, float accelY, float accelZ, float gyroRate,
                      bool useLowPass, bool useHighPass, bool useMovingAvg);

    // Verarbeitung für Pitch
    float processPitch(float accelX, float accelY, float accelZ, float gyroRate,
                       bool useLowPass, bool useHighPass, bool useMovingAvg);

private:
    // Interne Methoden
    float calculateDeltaTime(); // Berechnet die Zeitdifferenz
    float applyFilters(float value, bool useLowPass, bool useHighPass, bool useMovingAvg); // Anwenden der Filter
    float complementaryFusion(float accelAngle, float gyroRate, float dt, float alpha); // Komplementär-Filter

    // Filter
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;

    // PID-Controller
    PID& rollPID;
    PID& pitchPID;

    // Schwerpunkt-Offsets
    float cgOffsetX, cgOffsetY, cgOffsetZ;

    // Zeitmessung
    unsigned long lastUpdateTime;

    // Fusion-Parameter
    float alpha;
};

#endif // FILTER_HANDLER_H
