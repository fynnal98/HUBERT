#include "TailRotor.h"

TailRotor::TailRotor(int motorPin, float scaleFactor, PID& pidYaw)
    : motorPin(motorPin), scaleFactor(scaleFactor),  pidYaw(pidYaw), correctionEnabled(true), lastYawSetpoint(0) {}

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
    // Skaliere den Eingabewert von channel8Pulse, um größere scaleFactor-Werte zu ermöglichen
    float normalizedChannel8Pulse = map(channel8Pulse, 1000, 2000, 500, 1500);  // Skaliere auf einen kleineren Bereich

    // Berechne den Basiswert für den Heckrotor
    unsigned long basePulse = normalizedChannel8Pulse * scaleFactor;

    // Berechne die manuelle Anpassung basierend auf channel4Pulse
    int manualAdjustment = map(channel4Pulse, 1000, 2000, -100, 100);

    // Füge die Yaw-Korrektur und die manuelle Anpassung hinzu
    unsigned long totalPulse = basePulse - yawCorrection + manualAdjustment;


    // Begrenze den Wert auf den gültigen Bereich (1000-2000 Mikrosekunden)
    return constrain(totalPulse, 1000, 2000);
}
