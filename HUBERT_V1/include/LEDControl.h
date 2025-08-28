#ifndef LEDCONTROL_H
#define LEDCONTROL_H

#include <Arduino.h>

class LEDControl {
public:
    // Konstruktor mit Angabe des LED-Pins
    LEDControl(int pin);
    
    // Methode zum Einschalten der LED
    void steadyOn();
    
    // Methode zum Ausschalten der LED
    void steadyOff();
    
    // Methode zum Blinken der LED
    void blink(int interval);

private:
    int ledPin;
    unsigned long previousMillis;
    bool ledState;
};

#endif // LEDCONTROL_H
