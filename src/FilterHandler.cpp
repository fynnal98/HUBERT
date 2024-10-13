#include "FilterHandler.h"
#include "ParameterHandler.h"
#include "DataLogger.h"

// Konstruktor: Initialisiert alle Filter und den PID-Regler
FilterHandler::FilterHandler(float lowPassAlpha, float highPassAlpha, int movingAvgWindowSize, float kalmanQ, float kalmanR, float kalmanEstimateError, float kalmanInitialEstimate, PID& pid)
    : lowPassFilter(lowPassAlpha), highPassFilter(highPassAlpha),
      movingAvgFilter(movingAvgWindowSize), kalmanFilter(kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate),
      rpmFilter(), pid(pid) {
    // Setze initial RPM für den RPM-Filter
    rpmFilter.setRPM(getRPM());
}

// Filter anwenden und das Ergebnis loggen
float FilterHandler::apply(float value, bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman, bool useRPMFilter, DataLogger& logger) {
    float filteredValue = value;  // Starte mit dem Rohwert
    float initialValue = value;   // Ungefilterter Wert

    // RPM-Filter anwenden, wenn aktiviert
    if (useRPMFilter) {
        filteredValue = rpmFilter.apply(filteredValue);
    }

    // Low-pass Filter anwenden, wenn aktiviert
    if (useLowPass) {
        filteredValue = lowPassFilter.apply(filteredValue);
    }

    // High-pass Filter anwenden, wenn aktiviert
    if (useHighPass) {
        filteredValue = highPassFilter.apply(filteredValue);
    }

    // Moving Average Filter anwenden, wenn aktiviert
    if (useMovingAvg) {
        filteredValue = movingAvgFilter.apply(filteredValue);
    }

    // Kalman Filter anwenden, wenn aktiviert
    if (useKalman) {
        filteredValue = kalmanFilter.updateEstimate(filteredValue);
    }

    // Logge jetzt sowohl Rohdaten als auch gefilterte Daten synchron nach allen Berechnungen
    logger.logData(initialValue, initialValue, filteredValue, filteredValue);

    // PID-Korrektur anwenden und das finale Ergebnis zurückgeben
    return pid.compute(0, filteredValue);
}
