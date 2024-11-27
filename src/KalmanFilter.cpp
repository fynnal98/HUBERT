#include "KalmanFilter.h"

KalmanFilter::KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate)
    : processNoise(processNoise), measurementNoise(measurementNoise), 
      estimateError(estimateError), lastEstimate(initialEstimate) {}

float KalmanFilter::update(float predictedValue, float measuredValue, float dt) {
    // Erhöhe den Schätzfehler basierend auf dem Prozessrauschen und der Zeit
    estimateError += processNoise * dt;

    // Berechne den Kalman-Gewinn
    float kalmanGain = estimateError / (estimateError + measurementNoise);

    // Aktualisiere die Schätzung basierend auf der Vorhersage (Gyro) und der Messung (Accel)
    lastEstimate = predictedValue + kalmanGain * (measuredValue - predictedValue);

    // Aktualisiere den Schätzfehler
    estimateError *= (1 - kalmanGain);

    return lastEstimate;
}
