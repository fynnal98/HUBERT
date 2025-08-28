#ifndef FBL_H
#define FBL_H

#include <ESP32Servo.h>
#include "FilterHandler.h"
#include "DataLogger.h"
#include "MPU6050.h"

class FBL {
public:
    // Konstruktor
    FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler, DataLogger& logger);

    // Initialisierung
    void setup();

    // Steuerung mit Filterkorrekturen
    void update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse,
                bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman);

    // Direkte Steuerung der Servos
    void directServoControl(unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse);

private:
    // Servopins
    int servo1Pin, servo2Pin, servo3Pin;

    // Servos
    Servo servo1, servo2, servo3;

    // Filterhandler für Roll und Pitch
    FilterHandler& rollFilterHandler;
    FilterHandler& pitchFilterHandler;

    // Logger
    DataLogger& logger;

    // Letzter Zeitstempel für dt-Berechnung
    unsigned long lastUpdateTime;
};

#endif // FBL_H
