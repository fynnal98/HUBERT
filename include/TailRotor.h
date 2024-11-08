#ifndef TAILROTOR_H
#define TAILROTOR_H

#include <ESP32Servo.h>
#include "PID.h"

class TailRotor {
public:
    TailRotor(int motorPin, float scaleFactor, PID& pidYaw);  // Übergibt den PID-Controller als Referenz
    void setup();
    void update(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawRate);
    
    // Methode, um die Korrektur für den Heckrotor ein- oder auszuschalten
    void setCorrectionEnabled(bool enabled);

private:
    int motorPin;
    float scaleFactor;
    Servo motorServo;
    PID& pidYaw;  // Speichere den PID-Regler als Referenz
    bool correctionEnabled;  // Variable zur Steuerung der Korrektur

    // Berechnet den PWM-Wert für den Heckrotor basierend auf der Korrektur
    unsigned long computeTailMotorPulse(unsigned long channel8Pulse, unsigned long channel4Pulse, float yawCorrection);
};

#endif  // TAILROTOR_H
