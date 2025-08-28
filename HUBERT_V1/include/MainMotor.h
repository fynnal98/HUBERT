#ifndef MAINMOTOR_H
#define MAINMOTOR_H

#include <ESP32Servo.h>

class MainMotor {
public:
    // Konstruktor
    MainMotor(int motorPin);

    // Setup-Funktion für den Motor
    void setup();

    // Funktion zum Steuern des Motors
    void setPulse(unsigned long pulse);

private:
    Servo motorServo;  // Servo-Objekt für den Hauptmotor
    int motorPin;      // Pin für den Hauptmotor
};

#endif  // MAINMOTOR_H
