#include "FBL.h"
#include "ParameterHandler.h"  // Importiere ParameterHandler

// Konstruktor
FBL::FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler, DataLogger& logger)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3),
      rollFilterHandler(rollFilterHandler), pitchFilterHandler(pitchFilterHandler),
      logger(logger), lastUpdateTime(0) {}

// Initialisiere die Servos
void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

// Direkte Steuerung der Servos (ohne Filterkorrekturen)
void FBL::directServoControl(unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse) {
    servo1.writeMicroseconds(channel2Pulse);
    servo2.writeMicroseconds(channel6Pulse);
    servo3.writeMicroseconds(channel1Pulse);
}

// Aktualisierung der Steuerung mit Filterkorrekturen
void FBL::update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse,
                 bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman) {
    // Zeitschritt berechnen
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdateTime) / 1000.0; // Umrechnung in Sekunden
    lastUpdateTime = currentTime;

    // Sensordaten abrufen
    sensors_event_t a, g, temp;  // Dummy für Temperaturdaten
    mpu.getEvent(&a, &g, &temp);

    float rollCorrection = rollFilterHandler.processRoll(a.acceleration.x, a.acceleration.y, a.acceleration.z, g.gyro.x, useLowPass, useHighPass, useMovingAvg);
    float pitchCorrection = pitchFilterHandler.processPitch(a.acceleration.x, a.acceleration.y, a.acceleration.z, g.gyro.y, useLowPass, useHighPass, useMovingAvg);

    // Anwenden der Korrekturen auf die Servokanäle
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Right

    servo1Pulse = constrain(servo1Pulse, 1000, 2000);
    servo2Pulse = constrain(servo2Pulse, 1000, 2000);
    servo3Pulse = constrain(servo3Pulse, 1000, 2000);

    // Servopulse setzen
    servo1.writeMicroseconds(servo1Pulse);
    servo2.writeMicroseconds(servo2Pulse);
    servo3.writeMicroseconds(servo3Pulse);
}
