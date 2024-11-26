#include <Arduino.h>
#include <ESP32Servo.h>
#include "SBUSReceiver.h"
#include "MPU6050.h"
#include "MPU6050Calibration.h"
#include "PID.h"
#include "FBL.h"
#include "WDT.h"
#include "MainMotor.h"
#include "Util.h"
#include "TailRotor.h"
#include "FilterHandler.h"
#include "DataLogger.h"
#include "ParameterHandler.h"
#include "SerialHandler.h"
#include "LEDControl.h" 
#include <SPIFFS.h>

// MPU
MPU6050 mpu;

// Logger erstellen
DataLogger logger;

// LED-Steuerung
extern int ledPin;
LEDControl ledControl(ledPin);

// FilterHandler für Roll und Pitch erstellen
FilterHandler rollFilterHandler(lowPassCutoffFrequency, highPassCutoffFrequency, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidRoll);
FilterHandler pitchFilterHandler(lowPassCutoffFrequency, highPassCutoffFrequency, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidPitch);

// FBL-Objekt
FBL fbl(pinServo1, pinServo2, pinServo3, rollFilterHandler, pitchFilterHandler, logger);

// Motoren
MainMotor mainMotorServo(mainMotorPin);
TailRotor tailRotor(tailMotorPin, scaleFactor, pidYaw);


SBUSReceiver sbusReceiver(Serial2);

void listSPIFFSFiles() {
    Serial.println("Liste der Dateien im SPIFFS:");
    File root = SPIFFS.open("/");
    File file = root.openNextFile();
    while (file) {
        Serial.printf("Datei: %s\n", file.name());
        file = root.openNextFile();
    }
}

void setup() {
    Serial.begin(115200);

    SPIFFS.begin();
    listSPIFFSFiles();

    const char* filePath = "/SystemDatabase/database.json";
    
    initializeParametersFromJSON(filePath);

    initWatchdog(2);

    sbusReceiver.begin();

    Wire.begin(wireSDA, wireSCL);

    mpu.begin();

    MPU6050Calibration::beginCalibration(calibrationDuration);

    fbl.setup();
    tailRotor.setup();
    mainMotorServo.setup();

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel3Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel9Pulse, channel10Pulse;

    processSerialData();
    resetWatchdog();
    mpu.checkConnectionAndBlink();

    if (!calibrationCompleted) {
        calibrationCompleted = MPU6050Calibration::updateCalibration(mpu, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        if (calibrationCompleted) {
            Serial.println("Gyroskop-Kalibrierung abgeschlossen");
        }
        return;
    }

    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel3Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel9Pulse, channel10Pulse)) {
        // Deklariere die Event-Variablen
        sensors_event_t a, g, temp;

        // Rufe die Daten des MPU ab
        mpu.getEvent(&a, &g, &temp);

        // Prüfe, ob die Korrektur für die FBL aktiviert ist
        if (Util::correctionEnabled(channel10Pulse) && mpu.isConnected()) {
            // Aktiviert die Korrektur für die Swashplate
            fbl.update(mpu, channel1Pulse, channel2Pulse, channel6Pulse, useLowPass, useHighPass, useMovingAvg, useKalman);
            ledControl.steadyOn();
        } else {
            // Deaktiviert die Korrektur und steuert die Servos direkt
            fbl.servo1.writeMicroseconds(channel2Pulse);
            fbl.servo2.writeMicroseconds(channel6Pulse);
            fbl.servo3.writeMicroseconds(channel1Pulse);
            ledControl.steadyOff();
        }

        // Prüfe, ob die Korrektur für den Heckrotor aktiviert ist
        if (channel9Pulse > 1500 && mpu.isConnected()) {
            tailRotor.setCorrectionEnabled(true);
        } else {
            tailRotor.setCorrectionEnabled(false);
        }

        // Update des Heckrotors (mit oder ohne Korrektur)
        tailRotor.update(channel8Pulse, channel4Pulse, g.gyro.z);

        // Setze den MainMotor-Puls
        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(15);
}
