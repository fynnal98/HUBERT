#include "TailRotor.h"

TailRotor::TailRotor(int motorPin, float scaleFactor, float pitchFactor, PID& pidYaw)
    : motorPin(motorPin), scaleFactor(scaleFactor), pitchFactor(pitchFactor), pidYaw(pidYaw), correctionEnabled(true), lastYawSetpoint(0) {}

void TailRotor::setup() {
    motorServo.attach(motorPin);
}

void TailRotor::setCorrectionEnabled(bool enabled) {
    correctionEnabled = enabled;
}

void TailRotor::update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate) {
    float yawCorrection = 0;

    // Erkenne manuelle Eingriffe am Yaw-Kanal
    bool manualYawControl = abs((int)channel4Pulse - 1500) > 50; // Toleranzbereich von ±50 um den Mittelwert 1500

    if (manualYawControl) {
        // Wenn manuell gesteuert wird, PID-Korrektur aussetzen und aktuellen Yaw-Wert als Sollwert setzen
        lastYawSetpoint = yawRate;  // Der aktuelle Yaw-Wert wird als neuer Sollwert gesetzt
    }

    // Korrektur nur berechnen, wenn sie aktiviert ist
    if (correctionEnabled) {
        pidYaw.setSetpoint(lastYawSetpoint); // Setze den letzten Yaw-Wert als Sollwert für den PID-Regler
        yawCorrection = pidYaw.compute(lastYawSetpoint, yawRate);
    }

    unsigned long adjustedTailMotorPulse = computeTailMotorPulse(channel8Pulse, channel4Pulse, yawCorrection);

    motorServo.writeMicroseconds(adjustedTailMotorPulse);
}


unsigned long TailRotor::computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection) {
    // Grundpuls basierend auf channel8Pulse, skaliert mit scaleFactor
    float basePulse = channel8Pulse * scaleFactor;

    // Manuelle Anpassung basierend auf channel4Pulse
    int manualAdjustment = map(channel4Pulse, 1000, 2000, -200, 200);

    // Gesamtberechnung des Pulses unter Berücksichtigung aller Faktoren
    unsigned long totalPulse = basePulse - yawCorrection + manualAdjustment;

    // Begrenzung des Pulses auf den Bereich von 1000 bis 2000 Mikrosekunden
    return constrain(totalPulse, 1000, 2000);
}
