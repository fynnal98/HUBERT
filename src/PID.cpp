#include "PID.h"

PID::PID(float kp, float ki, float kd, float integralLimit, float factor)
    : kp(kp), ki(ki), kd(kd), prevError(0), integral(0), integralLimit(integralLimit), factor(factor) {}

float PID::compute(float setpoint, float measured) {
    float error = setpoint - measured;

    // Berechnung des Integrals (mit Begrenzung)
    integral += error;
    if (integral > integralLimit) {
        integral = integralLimit;
    } else if (integral < -integralLimit) {
        integral = -integralLimit;
    }

    // Berechnung der Ableitung
    float derivative = error - prevError;
    prevError = error;

    // PID-Ausgabe und Faktor anwenden
    return (kp * error + ki * integral + kd * derivative) * factor;
}

void PID::setSetpoint(float setpoint) {
    prevError = setpoint; // Setze den letzten Fehler auf den neuen Sollwert
    integral = 0; // Setze das Integral zurück, um eine frische Berechnung zu starten
}
