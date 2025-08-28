#ifndef DRONELOGIC_H
#define DRONELOGIC_H

#include <Arduino.h>
#include <ESP32Servo.h>
#include "DatabaseTool.h"
#include "SensorHandler.h"
#include "InputHandler.h"

class DroneLogic {
public:
    DroneLogic();
    void init(DatabaseTool* db);   // Initialisierung
    void update(SensorHandler* sensors, InputHandler* input); // Hauptsteuerung

private:
    Servo motors[4];       // Array von Servos für die Motorsteuerung
    int motorPins[4];      // Pins für die Motorsteuerung
    int motorSpeeds[4];    // PWM-Werte der Motoren (1000–2000 µs)

    // PID-Parameter
    float kp, ki, kd;

    // Roll- und Pitch-Korrektur (Fehlerwerte)
    float rollErrorSum = 0;   // Summierter Fehler für Roll (Integral)
    float pitchErrorSum = 0;  // Summierter Fehler für Pitch (Integral)
    float lastRollError = 0;  // Letzter Fehler für Roll (Ableitung)
    float lastPitchError = 0; // Letzter Fehler für Pitch (Ableitung)
};

#endif // DRONELOGIC_H
