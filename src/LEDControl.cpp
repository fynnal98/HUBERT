#include "LEDControl.h"

// Konstruktor: Initialisiert den LED-Pin und den Ausgangsstatus
LEDControl::LEDControl(int pin) : ledPin(pin), previousMillis(0), ledState(false) {
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, LOW);
}

// Schaltet die LED dauerhaft ein
void LEDControl::steadyOn() {
    digitalWrite(ledPin, HIGH);
    ledState = true;
}

// Schaltet die LED dauerhaft aus
void LEDControl::steadyOff() {
    digitalWrite(ledPin, LOW);
    ledState = false;
}

// Lässt die LED blinken in Intervallen (Millisekunden)
void LEDControl::blink(int interval) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        ledState = !ledState;
        digitalWrite(ledPin, ledState ? HIGH : LOW);
    }
}
