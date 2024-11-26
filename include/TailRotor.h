// TailRotor.h - Header-Datei für die TailRotor-Klasse

#ifndef TAILROTOR_H
#define TAILROTOR_H

#include <ESP32Servo.h>
#include "PID.h"

class TailRotor {
public:
    TailRotor(int motorPin, float scaleFactor, PID& pidYaw); // pitchFactor hier behalten
    void setup();
    void update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate); // Entfernt channel3Pulse
    void setCorrectionEnabled(bool enabled);

private:
    int motorPin;

    // Tail-spezifische Faktoren
    float scaleFactor;
    float lastYawSetpoint;

    Servo motorServo;
    PID& pidYaw;
    bool correctionEnabled;

    unsigned long computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection); // Entfernt channel3Pulse
};

#endif // TAILROTOR_H
