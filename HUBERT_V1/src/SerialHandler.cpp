#include "SerialHandler.h"
#include "ParameterHandler.h"
#include <Arduino.h>  // String benötigt Arduino.h

// Funktion, um serielle Daten zu verarbeiten
void processSerialData() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');

        // Beispiel-String: rollPID=80.0;pitchPID=80.0;yawPID=5.0;servoPinAft=13;mainMotorPin=22
        if (input.indexOf("rollPID=") != -1) {
            rollPIDFactor = extractParameterValue(input, "rollPID=");
            Serial.print("Neuer Roll PID: ");
            Serial.println(rollPIDFactor);  // Debugging: Neuen Wert ausgeben
        }
        if (input.indexOf("pitchPID=") != -1) {
            pitchPIDFactor = extractParameterValue(input, "pitchPID=");
        }
        if (input.indexOf("yawPID=") != -1) {
            yawPIDFactor = extractParameterValue(input, "yawPID=");
        }
        if (input.indexOf("lowPassAlpha=") != -1) {
            lowPassCutoffFrequency = extractParameterValue(input, "lowPassAlpha=");
        }
        if (input.indexOf("highPassAlpha=") != -1) {
            highPassCutoffFrequency = extractParameterValue(input, "highPassAlpha=");
        }
        if (input.indexOf("movingAvgWindowSize=") != -1) {
            movingAvgWindowSize = (int)extractParameterValue(input, "movingAvgWindowSize=");
        }
        if (input.indexOf("kalmanQ=") != -1) {
            kalmanQ = extractParameterValue(input, "kalmanQ=");
        }
        if (input.indexOf("kalmanR=") != -1) {
            kalmanR = extractParameterValue(input, "kalmanR=");
        }
        if (input.indexOf("gyroOffsetX=") != -1) {
            gyroDriftOffsetX = extractParameterValue(input, "gyroOffsetX=");
        }
        if (input.indexOf("gyroOffsetY=") != -1) {
            gyroDriftOffsetY = extractParameterValue(input, "gyroOffsetY=");
        }
        if (input.indexOf("gyroOffsetZ=") != -1) {
            gyroDriftOffsetZ = extractParameterValue(input, "gyroOffsetZ=");
        }
        
        // Neuer Code für die Pins
        if (input.indexOf("servoPinAft=") != -1) {
            pinServo1 = (int)extractParameterValue(input, "servoPinAft=");
            Serial.print("Neuer Servo Pin Aft: ");
            Serial.println(pinServo1);
        }
        if (input.indexOf("servoPinLeft=") != -1) {
            pinServo2 = (int)extractParameterValue(input, "servoPinLeft=");
            Serial.print("Neuer Servo Pin Left: ");
            Serial.println(pinServo2);
        }
        if (input.indexOf("servoPinRight=") != -1) {
            pinServo3 = (int)extractParameterValue(input, "servoPinRight=");
            Serial.print("Neuer Servo Pin Right: ");
            Serial.println(pinServo3);
        }
        // if (input.indexOf("mainMotorPin=") != -1) {
        //     mainMotorPin = (int)extractParameterValue(input, "mainMotorPin=");
        //     Serial.print("Neuer Main Motor Pin: ");
        //     Serial.println(mainMotorPin);
        // }
        // if (input.indexOf("tailMotorPin=") != -1) {
        //     tailMotorPin = (int)extractParameterValue(input, "tailMotorPin=");
        //     Serial.print("Neuer Tail Motor Pin: ");
        //     Serial.println(tailMotorPin);
        // }
        // if (input.indexOf("sbusPin=") != -1) {
        //     sbusPin = (int)extractParameterValue(input, "sbusPin=");
        //     Serial.print("Neuer SBUS Pin: ");
        //     Serial.println(sbusPin);
        // }
        // if (input.indexOf("sdaPin=") != -1) {
        //     wireSDA = (int)extractParameterValue(input, "sdaPin=");
        //     Serial.print("Neuer SDA Pin: ");
        //     Serial.println(wireSDA);
        // }
        // if (input.indexOf("sclPin=") != -1) {
        //     wireSCL = (int)extractParameterValue(input, "sclPin=");
        //     Serial.print("Neuer SCL Pin: ");
        //     Serial.println(wireSCL);
        // }

        Serial.println("Parameter wurden erfolgreich aktualisiert.");
    }
}

// Funktion, um den Wert eines Parameters aus dem String zu extrahieren
float extractParameterValue(const String& input, const String& parameterName) {
    int startIndex = input.indexOf(parameterName) + parameterName.length();
    int endIndex = input.indexOf(';', startIndex);
    String valueString = input.substring(startIndex, endIndex);
    return valueString.toFloat();
}
