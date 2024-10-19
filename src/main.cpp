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


// MPU
MPU6050 mpu;

// Logger erstellen
DataLogger logger;

// FilterHandler für Roll und Pitch erstellen
FilterHandler rollFilterHandler(lowPassAlpha, highPassAlpha, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidRoll);
FilterHandler pitchFilterHandler(lowPassAlpha, highPassAlpha, movingAvgWindowSize, kalmanQ, kalmanR, kalmanEstimateError, kalmanInitialEstimate, pidPitch);

// FBL-Objekt
FBL fbl(pinServo1, pinServo2, pinServo3, rollFilterHandler, pitchFilterHandler, logger);

// Motoren
MainMotor mainMotorServo(mainMotorPin);
TailRotor tailRotor(tailMotorPin, tailRotorFactor, pidYaw);  // Tailrotor-Faktor
SBUSReceiver sbusReceiver(Serial2);  // Initialisiere mit Serial2

void setup() {
    Serial.begin(115200);

    // Initialisiere Parameter
    initializeParameters();

    initWatchdog(2);
    sbusReceiver.begin();
    Wire.begin(wireSDA, wireSCL);  // Verwende die Pins für SDA und SCL aus dem ParameterHandler
    mpu.begin();
    mpu.setup();

    Serial.println("Starte Gyroskop-Kalibrierung...");
    MPU6050Calibration::beginCalibration(calibrationDuration);

    fbl.setup();
    tailRotor.setup();
    mainMotorServo.setup();

    Serial.println("Setup abgeschlossen");
}

void loop() {
    unsigned int channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse;

    processSerialData();
    resetWatchdog();

    if (!calibrationCompleted) {
        calibrationCompleted = MPU6050Calibration::updateCalibration(mpu, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        if (calibrationCompleted) {
            Serial.println("Gyroskop-Kalibrierung abgeschlossen");
        }
        return;
    }

    if (sbusReceiver.readChannels(channel1Pulse, channel2Pulse, channel4Pulse, channel6Pulse, channel8Pulse, channel10Pulse)) {
        sensors_event_t a, g, temp;
        mpu.getEvent(&a, &g, &temp);
        mpu.applyGyroOffset(g, gyroDriftOffsetX, gyroDriftOffsetY, gyroDriftOffsetZ);
        float yawRate = g.gyro.z;

        if (Util::correctionEnabled(channel10Pulse)) {
            // Übergib die Filter-Flags und wende die Filter an
            fbl.update(mpu, channel1Pulse, channel2Pulse, channel6Pulse, useLowPass, useHighPass, useMovingAvg, useKalman);
            tailRotor.update(channel8Pulse, channel4Pulse, yawRate);
        } else {
            fbl.servo1.writeMicroseconds(channel2Pulse);
            fbl.servo2.writeMicroseconds(channel6Pulse);
            fbl.servo3.writeMicroseconds(channel1Pulse);

            tailRotor.update(channel8Pulse, channel4Pulse, 0);
        }

        mainMotorServo.setPulse(channel8Pulse);
    } else {
        Serial.println("Fehler beim Lesen der Kanäle.");
    }

    delay(15);
}
