#ifndef KALMANFILTER_H
#define KALMANFILTER_H

class KalmanFilter {
public:
    KalmanFilter(float processNoise, float measurementNoise, float estimateError, float initialEstimate);

    // Aktualisiere die Schätzung basierend auf Vorhersage (Gyro) und Messung (Accelerometer)
    float update(float predictedValue, float measuredValue, float dt);

private:
    float processNoise;      // Prozessrauschen (Q)
    float measurementNoise;  // Messrauschen (R)
    float estimateError;     // Fehler in der Schätzung (P)
    float lastEstimate;      // Letzte Schätzung
};

#endif // KALMANFILTER_H
