#include "ParameterHandler.h"

// Definition der Servo-Pins
int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

// PID-Controller
PID pidRoll(80.0, 0.0, 5, integralLimit, rollPIDFactor);
PID pidPitch(80.0, 0.0, 5, integralLimit, pitchPIDFactor);
PID pidYaw(90.0, 0.0, 5.0, integralLimit, yawPIDFactor);

float rollPIDFactor = 1;
float pitchPIDFactor = 1;
float yawPIDFactor = 1.5;
float integralLimit = 100.0;

// Motor-Pins
const int mainMotorPin = 5;
const int tailMotorPin = 17;
const int tailRotorFactor = 1;

// SBUS-Pin
const int sbusPin = 16;

// Wire-Pins (SDA, SCL)
const int wireSDA = 21;
const int wireSCL = 22;

// Filterparameter
float lowPassAlpha = 0.2;
float highPassAlpha = 0.999;
int movingAvgWindowSize = 2;
float kalmanQ = 0.02;
float kalmanR = 0.2;
float kalmanEstimateError = 1.0;
float kalmanInitialEstimate = 0.0;

// RPM für den RPM-Filter
int rpm = 20000;
float bandwidth = 10.0;

// Flags, um Filter zu aktivieren oder deaktivieren
bool useLowPass = true;
bool useHighPass = true;
bool useMovingAvg = false;
bool useKalman = false;
bool useRPMFilter = true; 

// CG-Offsets für den MPU
float cgOffsetX = -0.09;
float cgOffsetY = 0.0;
float cgOffsetZ = 0.0;

// Gyro Drift-Offsets
float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

// Kalibrierungsparameter
const int calibrationDuration = 1000;

// Kalibrierungsstatus
bool calibrationCompleted = false;  // Setze den initialen Status auf 'false'



void initializeParameters() {
    // Falls zusätzliche Initialisierungslogik benötigt wird, kann sie hier hinzugefügt werden.
}

void setRPM(int newRPM) {
    rpm = newRPM;
}

int getRPM() {
    return rpm;
}
