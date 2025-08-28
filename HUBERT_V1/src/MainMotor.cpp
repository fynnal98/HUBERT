#include "MainMotor.h"
#include <Arduino.h>

// Konstruktor: Pin für den Motor übergeben
MainMotor::MainMotor(int motorPin) : motorPin(motorPin) {}

// Setup-Funktion: Initialisiert den Servo und verbindet ihn mit dem Pin
void MainMotor::setup() {
    motorServo.attach(motorPin);  // Verbindet den Servo mit dem Pin
}

// SetPulse-Funktion: Stellt den PWM-Wert (Pulse) ein
void MainMotor::setPulse(unsigned long pulse) {
    motorServo.writeMicroseconds(pulse);  // Sendet den PWM-Wert an den Servo
}
