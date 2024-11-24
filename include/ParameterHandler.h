#ifndef PARAMETERHANDLER_H
#define PARAMETERHANDLER_H

#include <string>

#include "PID.h"

bool initializeParametersFromJSON(const char* filePath);

// Definition der Servo-Pins
extern int pinServo1;
extern int pinServo2;
extern int pinServo3;

// PID-Controller
extern PID pidRoll;
extern PID pidPitch;
extern PID pidYaw;

extern float rollPIDFactor;
extern float pitchPIDFactor;
extern float yawPIDFactor;
extern float integralLimit;

// Motor-Pins
extern int mainMotorPin;
extern int tailMotorPin;

extern float scaleFactor;
extern float pitchFactor;

// SBUS
extern int sbusPin;

// Wire-Pins (SDA, SCL)
extern int wireSDA;
extern int wireSCL;

extern float sampleRate; 

// Filterparameter
extern float lowPassCutoffFrequency;
extern float highPassCutoffFrequency;

extern int movingAvgWindowSize;
extern float kalmanQ;
extern float kalmanR;
extern float kalmanEstimateError;
extern float kalmanInitialEstimate;

// Flags, um Filter zu aktivieren oder deaktivieren
extern bool useLowPass;
extern bool useHighPass;
extern bool useMovingAvg;
extern bool useKalman;

// CG-Offsets für den MPU
extern float cgOffsetX;
extern float cgOffsetY;
extern float cgOffsetZ;

// Gyro Drift-Offsets
extern float gyroDriftOffsetX;
extern float gyroDriftOffsetY;
extern float gyroDriftOffsetZ;

// Kalibrierungsparameter
extern int calibrationDuration;

// Kalibrierungsstatus
extern bool calibrationCompleted;


// LED-Pin für Statusanzeigen (wie z. B. Kalibrierung abgeschlossen)
extern int ledPin;  // LED-Pin über ParameterHandler verwaltet

void initializeParameters();

#endif // PARAMETERHANDLER_H
