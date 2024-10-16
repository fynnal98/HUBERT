#ifndef FILTER_HANDLER_H
#define FILTER_HANDLER_H

#include "LowPassFilter.h"
#include "HighPassFilter.h"
#include "MovingAverageFilter.h"
#include "KalmanFilter.h"
#include "PID.h"
#include "DataLogger.h"
#include "RPMFilter.h"

class FilterHandler {
public:
    FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid);
    float apply(float value, bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman, bool useRPMFilter, DataLogger& logger);

private:
    LowPassFilter lowPassFilter;
    HighPassFilter highPassFilter;
    MovingAverageFilter movingAvgFilter;
    KalmanFilter kalmanFilter;
    RPMFilter rpmFilter;
    PID& pid;
};

#endif // FILTER_HANDLER_H
