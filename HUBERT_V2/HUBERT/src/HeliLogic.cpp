#include "HeliLogic.h"
#include <Arduino.h>

HeliLogic::HeliLogic(DatabaseTool* db) : database(db), lastUpdateTime(0), iTerm(0.0f), lastRollError(0.0f), lastPitchError(0.0f) {}

void HeliLogic::init() {
    Serial.println("Initialisiere HeliLogic...");

    // Pins aus der Datenbank laden
    motorMainPin = database->get<int>("mode/heli/MotorMain", 5);
    motorAftPin = database->get<int>("mode/heli/MotorAft", 17);
    servoBackPin = database->get<int>("mode/heli/ServoBack", 13); // Servo 2
    servoLeftPin = database->get<int>("mode/heli/ServoLeft", 14); // Servo 6
    servoRightPin = database->get<int>("mode/heli/ServoRight", 15); // Servo 1

    // PID-Parameter laden
    kp = database->get<float>("mode/heli/pid/kp", 1.0f);
    ki = database->get<float>("mode/heli/pid/ki", 0.0f);
    kd = database->get<float>("mode/heli/pid/kd", 0.0f);

    // Yaw-Faktor laden
    yawFactor = database->get<float>("mode/heli/yawFactor", 1.0f);

    // Servos initialisieren
    servoBack.attach(servoBackPin);
    servoLeft.attach(servoLeftPin);
    servoRight.attach(servoRightPin);

    // Hauptmotor initialisieren
    mainMotor.attach(motorMainPin);
    aftMotor.attach(motorAftPin);

    Serial.println("HeliLogic initialisiert.");

    Serial.printf("Yaw-Faktor geladen: %.2f\n", yawFactor);
}

void HeliLogic::update(SensorHandler* sensors, InputHandler* input) {
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    // Eingabekanäle
    unsigned long rollInput = map(input->getChannelValue("roll"), 172, 1811, -500, 500);
    unsigned long pitchInput = map(input->getChannelValue("pitch"), 172, 1811, -500, 500); 
    unsigned long thrustInput = map(input->getChannelValue("thrust"), 172, 1811, 1000, 2000); 
    unsigned long yawInput = map(input->getChannelValue("yaw"), 172, 1811, 1000, 2000); 
    unsigned long motorMainInput = map(input->getChannelValue("MotorMain"), 172, 1811, 1000, 2000); 

    // Hauptmotor steuern
    controlMainMotor(motorMainInput);

    // Heckmotor Input berechnen
    unsigned long motorAftInput = calculateAftMotorInput(motorMainInput, yawInput);
    controlAftMotor(motorAftInput);

    // Sensordaten für Roll und Pitch-Korrekturen
    float roll, pitch, yaw;
    sensors->getFilteredData(roll, pitch, yaw);

    // PID-Korrekturen berechnen
    float rollCorrection = calculatePID(roll, lastRollError, dt);
    float pitchCorrection = calculatePID(pitch, lastPitchError, dt);

    // Swashplate-Positionen berechnen
    unsigned long servoBackPulse, servoLeftPulse, servoRightPulse;
    calculateSwashplatePositions(rollInput, pitchInput, thrustInput, rollCorrection, pitchCorrection,
                                  servoBackPulse, servoLeftPulse, servoRightPulse);

    // Servos ansteuern
    servoBack.writeMicroseconds(servoBackPulse);
    servoLeft.writeMicroseconds(servoLeftPulse);
    servoRight.writeMicroseconds(servoRightPulse);
}

unsigned long HeliLogic::calculateAftMotorInput(unsigned long motorMainInput, unsigned long yawInput) {
    // Der Heckmotor läuft parallel zum Hauptmotor
    float baseSpeed = motorMainInput;

    // Yaw-Input wird additiv berücksichtigt, multipliziert mit yawFactor
    float yawAdjustment = constrain(map(yawInput, 1000, 2000, -200, 200) * yawFactor, -200, 200);

    // Gesamtgeschwindigkeit des Heckmotors berechnen
    unsigned long aftMotorInput = constrain(baseSpeed + yawAdjustment, 1000, 2000);

    return aftMotorInput;
}

float HeliLogic::calculatePID(float error, float& lastError, float dt) {
    float pTerm = kp * error;
    iTerm += ki * error * dt;
    float dTerm = kd * (error - lastError) / dt;
    lastError = error;
    return pTerm + iTerm + dTerm;
}

void HeliLogic::calculateSwashplatePositions(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                                              float rollCorrection, float pitchCorrection,
                                              unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {

    applyInputs(rollInput, pitchInput, thrustInput, servoBackPulse, servoLeftPulse, servoRightPulse);

    applyCorrections(rollCorrection, pitchCorrection, servoBackPulse, servoLeftPulse, servoRightPulse);

    servoBackPulse = constrain(servoBackPulse, 1000, 2000);
    servoLeftPulse = constrain(servoLeftPulse, 1000, 2000);
    servoRightPulse = constrain(servoRightPulse, 1000, 2000);
}

void HeliLogic::applyInputs(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                             unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {
    servoBackPulse = thrustInput;
    servoLeftPulse = thrustInput;
    servoRightPulse = 2000 - (thrustInput - 1000);

    servoLeftPulse -= rollInput;
    servoRightPulse -= rollInput;

    servoBackPulse += pitchInput;
    servoLeftPulse -= pitchInput;
    servoRightPulse += pitchInput;
}

void HeliLogic::applyCorrections(float rollCorrection, float pitchCorrection,
                                  unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse) {
    servoLeftPulse -= rollCorrection;
    servoRightPulse -= rollCorrection;  

    servoBackPulse -= pitchCorrection;
    servoLeftPulse += pitchCorrection;
    servoRightPulse -= pitchCorrection; 
}

void HeliLogic::controlMainMotor(unsigned long motorMainInput) {
    mainMotor.writeMicroseconds(motorMainInput);
}

void HeliLogic::controlAftMotor(unsigned long motorAftInput) {
    aftMotor.writeMicroseconds(motorAftInput);
}
