#include "ParameterHandler.h"

#include <ArduinoJson.h>
#include <SPIFFS.h>


// Definition der Servo-Pins
int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

int ledPin = 2;  


// PID-Controller
PID pidRoll(90.0, 0.0, 10, integralLimit, rollPIDFactor);
PID pidPitch(90.0, 0.0, 10, integralLimit, pitchPIDFactor);
PID pidYaw(90.0, 0.0, 5.0, integralLimit, yawPIDFactor);

float rollPIDFactor = 1;
float pitchPIDFactor = 1;
float yawPIDFactor = 2;
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

// Abtastrate des MPU6050 (in Hz)
float sampleRate = 1000.0;  // Fester Parameter für die Abtastrate

// Filterfrequenzen (statt der Alpha-Werte)
float lowPassCutoffFrequency = 60;  // Grenzfrequenz des Low-Pass-Filters in Hz
float highPassCutoffFrequency = 100; // Grenzfrequenz des High-Pass-Filters in Hz

int movingAvgWindowSize = 5;
float kalmanQ = 0.02;
float kalmanR = 0.2;
float kalmanEstimateError = 1.0;
float kalmanInitialEstimate = 0.0;

// Flags, um Filter zu aktivieren oder deaktivieren
bool useLowPass = true;
bool useHighPass = false;
bool useMovingAvg = false;
bool useKalman = false;

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
bool calibrationCompleted = false;  


bool initializeParametersFromJSON(const char* filePath) {
    File file = SPIFFS.open(filePath, "r");
    if (!file) {
        Serial.printf("Fehler beim Öffnen der JSON-Datei: %s\n", filePath);
        return false;
    }

    String fileContent = file.readString();
    file.close();

    StaticJsonDocument<4096> doc;

    // Versuche die JSON-Daten zu deserialisieren
    DeserializationError error = deserializeJson(doc, fileContent);
    if (error) {
        Serial.printf("Fehler beim Parsen der JSON-Datei: %s\n", error.c_str());
        return false;
    }

    if (doc["pins"]["servo"]["pinServo1"].is<int>()) {
        int pinServo1 = doc["pins"]["servo"]["pinServo1"];
    }
    if (doc["pins"]["servo"]["pinServo2"].is<int>()) {
        int pinServo2 = doc["pins"]["servo"]["pinServo2"];
    }
    if (doc["pins"]["servo"]["pinServo3"].is<int>()) {
        int pinServo3 = doc["pins"]["servo"]["pinServo3"];
    }

    return true;
}
