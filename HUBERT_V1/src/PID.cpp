#include "PID.h"
#include <Arduino.h>

PID::PID(float kp, float ki, float kd, float integralLimit, float factor)
    : kp(kp), ki(ki), kd(kd), prevError(0), integral(0), integralLimit(integralLimit), factor(factor) {}

float PID::compute(float setpoint, float measured) {
    float error = setpoint - measured;

    // Serial.print("Error: ");
    // Serial.println(error);

    integral += error;
    if (integral > integralLimit) {
        integral = integralLimit;
    } else if (integral < -integralLimit) {
        integral = -integralLimit;
    }

    float derivative = error - prevError;
    prevError = error;

    float output = (kp * error + ki * integral + kd * derivative) * factor;

    // Serial.print("PID Output: ");
    // Serial.println(output);

    return output;
}


void PID::setSetpoint(float setpoint) {
    prevError = setpoint; // Setze den letzten Fehler auf den neuen Sollwert
    integral = 0;         // Setze das Integral zurück, um eine frische Berechnung zu starten
}
