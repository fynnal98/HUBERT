#include "ParameterHandler.h"

#include <ArduinoJson.h>
#include <SPIFFS.h>


// Definition der Servo-Pins
int pinServo1 = 13;
int pinServo2 = 14;
int pinServo3 = 15;

int ledPin = 2;  

// PID-Controller
PID pidRoll(2.0, 0.1, 1, integralLimit, rollPIDFactor);
PID pidPitch(2.0, 0.1, 1, integralLimit, pitchPIDFactor);
PID pidYaw(90.0, 0.0, 5.0, integralLimit, yawPIDFactor);

float rollPIDFactor = 1.2;
float pitchPIDFactor = 1.2;
float yawPIDFactor = 1.8;
float integralLimit = 100.0;

// Motor-Pins
int mainMotorPin = 5;
int tailMotorPin = 17;

float scaleFactor = 1.85;
float pitchFactor = 0.2;

// SBUS-Pin
int sbusPin = 16;

// Wire-Pins (SDA, SCL)
int wireSDA = 21;
int wireSCL = 22;

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
bool useKalman = true; 

// CG-Offsets für den MPU
float cgOffsetX = -0.09;
float cgOffsetY = 0.0;
float cgOffsetZ = 0.0;

// Gyro Drift-Offsets
float gyroDriftOffsetX = 0.0;
float gyroDriftOffsetY = 0.0;
float gyroDriftOffsetZ = 0.0;

// Kalibrierungsparameter
int calibrationDuration = 400;

// Kalibrierungsstatus
bool calibrationCompleted = false;  

// Funktionsprototypen
template <typename T>
void loadParameter(JsonVariant json, T& param, const char* key);

// Implementierung der Funktion
bool initializeParametersFromJSON(const char* filePath) {
    File file = SPIFFS.open(filePath, "r");
    if (!file) {
        Serial.printf("Fehler beim Öffnen der JSON-Datei: %s\n", filePath);
        return false;
    }

    // Inhalt der Datei lesen
    String fileContent = file.readString();
    Serial.println("Inhalt der JSON-Datei:");
    Serial.println(fileContent);
    file.close();

    // JSON-Dokument erstellen und deserialisieren
    StaticJsonDocument<4096> doc;
    DeserializationError error = deserializeJson(doc, fileContent);
    if (error) {
        Serial.printf("Fehler beim Parsen der JSON-Datei: %s\n", error.c_str());
        return false;
    }
    // Pins laden
    loadParameter(doc["pins"]["led"], ledPin, "ledPin");
    loadParameter(doc["pins"]["servo"], pinServo1, "pinServo1");
    loadParameter(doc["pins"]["servo"], pinServo2, "pinServo2");
    loadParameter(doc["pins"]["servo"], pinServo3, "pinServo3");

    loadParameter(doc["pins"]["motor"], mainMotorPin, "mainMotorPin");
    loadParameter(doc["pins"]["motor"], tailMotorPin, "tailMotorPin");

    loadParameter(doc["tail"], scaleFactor, "scaleFactor");
    loadParameter(doc["tail"], pitchFactor, "pitchFactor");

    loadParameter(doc["pins"]["sbus"], sbusPin, "sbusPin");
    loadParameter(doc["pins"]["wire"], wireSDA, "wireSDA");
    loadParameter(doc["pins"]["wire"], wireSCL, "wireSCL");

    // PID-Werte laden
    loadParameter(doc["pid"]["roll"], pidRoll.kp, "kp");
    loadParameter(doc["pid"]["roll"], pidRoll.ki, "ki");
    loadParameter(doc["pid"]["roll"], pidRoll.kd, "kd");


    loadParameter(doc["pid"]["pitch"], pidPitch.kp, "kp");
    loadParameter(doc["pid"]["pitch"], pidPitch.ki, "ki");
    loadParameter(doc["pid"]["pitch"], pidPitch.kd, "kd");


    loadParameter(doc["pid"]["yaw"], pidYaw.kp, "kp");
    loadParameter(doc["pid"]["yaw"], pidYaw.ki, "ki");
    loadParameter(doc["pid"]["yaw"], pidYaw.kd, "kd");

    // Filterparameter laden
    loadParameter(doc["filter"]["filterValues"], lowPassCutoffFrequency, "lowPassCutoffFrequency");
    loadParameter(doc["filter"]["filterValues"], highPassCutoffFrequency, "highPassCutoffFrequency");
    loadParameter(doc["filter"]["filterValues"], movingAvgWindowSize, "movingAvgWindowSize");

    loadParameter(doc["filter"]["filterValues"], kalmanQ, "kalmanQ");
    loadParameter(doc["filter"]["filterValues"], kalmanR, "kalmanR");
    loadParameter(doc["filter"]["filterValues"], kalmanEstimateError, "kalmanEstimateError");
    loadParameter(doc["filter"]["filterValues"], kalmanInitialEstimate, "kalmanInitialEstimate");

    loadParameter(doc["filter"]["flags"], useLowPass, "useLowPass");
    loadParameter(doc["filter"]["flags"], useHighPass, "useHighPass");
    loadParameter(doc["filter"]["flags"], useMovingAvg, "useMovingAvg");
    loadParameter(doc["filter"]["flags"], useKalman, "useKalman");

    // Gyro-Offsets laden
    loadParameter(doc["gyro"]["mpu_offsets"], cgOffsetX, "cgOffsetX");
    loadParameter(doc["gyro"]["mpu_offsets"], cgOffsetY, "cgOffsetY");
    loadParameter(doc["gyro"]["mpu_offsets"], cgOffsetZ, "cgOffsetZ");

    loadParameter(doc["gyro"]["drift_offsets"], gyroDriftOffsetX, "gyroDriftOffsetX");
    loadParameter(doc["gyro"]["drift_offsets"], gyroDriftOffsetY, "gyroDriftOffsetY");
    loadParameter(doc["gyro"]["drift_offsets"], gyroDriftOffsetZ, "gyroDriftOffsetZ");

    // Kalibrierungsparameter laden
    loadParameter(doc["gyro"]["calibration"], calibrationDuration, "duration");
    loadParameter(doc["gyro"]["calibration"], calibrationCompleted, "completed");

    // Konfigurationsparameter laden
    loadParameter(doc["config"], sampleRate, "sampleRate");


    Serial.println("Parameter erfolgreich geladen.");
    return true;
}

// Template-Funktion zum Laden von Parametern
template <typename T>
void loadParameter(JsonVariant json, T& param, const char* key) {
    if (json[key].is<T>()) {
        param = json[key].as<T>();
        Serial.printf("%s geladen: ", key);

        // Alternative ohne if constexpr
        if (std::is_same<T, int>::value || std::is_same<T, float>::value) {
            Serial.println(param);
        } else if (std::is_same<T, bool>::value) {
            Serial.println(param ? "true" : "false");
        }
    } else {
        Serial.printf("%s nicht gefunden. Standardwert wird verwendet.\n", key);
    }
}



// "pio run --target uploadfs"
// for uploading the database

