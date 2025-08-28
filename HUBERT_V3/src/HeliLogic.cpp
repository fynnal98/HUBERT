#include "HeliLogic.h"
#include <Arduino.h>

HeliLogic::HeliLogic(JsonHandler* jsonHandler, InputManager* inputManager)
    : m_jsonHandler(jsonHandler), m_inputManager(inputManager) {}

void HeliLogic::loadParameters() {
    Serial.println("Lade Helikopter-Parameter...");

    // PID-Parameter laden
    kp = m_jsonHandler->GetValueByKey<float>("config/modis/heli/pid/kp", 1.0f);
    ki = m_jsonHandler->GetValueByKey<float>("config/modis/heli/pid/ki", 0.0f);
    kd = m_jsonHandler->GetValueByKey<float>("config/modis/heli/pid/kd", 0.0f);

    yawFactor = m_jsonHandler->GetValueByKey<float>("config/modis/heli/yawFactor", 1.0f);

    // Pins aus der JSON-Konfiguration laden
    m_motorMainPin = m_jsonHandler->GetValueByKey<int>("config/modis/heli/MotorMain", -1);
    m_motorAftPin = m_jsonHandler->GetValueByKey<int>("config/modis/heli/MotorAft", -1);
    m_servoBackPin = m_jsonHandler->GetValueByKey<int>("config/modis/heli/ServoBack", -1);
    m_servoLeftPin = m_jsonHandler->GetValueByKey<int>("config/modis/heli/ServoLeft", -1);
    m_servoRightPin = m_jsonHandler->GetValueByKey<int>("config/modis/heli/ServoRight", -1);

    // Serial.printf("Kanäle zugeordnet: roll=%d, pitch=%d, thrust=%d, yaw=%d, motorMain=%d\n",
    //     m_motorMainPin, m_motorAftPin, m_servoBackPin, m_servoLeftPin, m_servoRightPin);

    // Servos und Motoren initialisieren
    servoBack.attach(m_servoBackPin);
    servoLeft.attach(m_servoLeftPin);
    servoRight.attach(m_servoRightPin);
    mainMotor.attach(m_motorMainPin);
    aftMotor.attach(m_motorAftPin);

    // Kanäle aus der JSON-Konfiguration laden
    m_rollChannel = m_jsonHandler->GetValueByKey<int>("config/modis/heli/Channels/roll", -1);
    m_pitchChannel = m_jsonHandler->GetValueByKey<int>("config/modis/heli/Channels/pitch", -1);
    m_thrustChannel = m_jsonHandler->GetValueByKey<int>("config/modis/heli/Channels/thrust", -1);
    m_yawChannel = m_jsonHandler->GetValueByKey<int>("config/modis/heli/Channels/yaw", -1);
    m_motorMainChannel = m_jsonHandler->GetValueByKey<int>("config/modis/heli/Channels/MotorMain",-1);

    // Serial.printf("Kanäle zugeordnet: roll=%d, pitch=%d, thrust=%d, yaw=%d, motorMain=%d\n",
    //               m_rollChannel, m_pitchChannel, m_thrustChannel, m_yawChannel, m_motorMainChannel);
}

unsigned long HeliLogic::mapChannelValue(int channel, int minInput, int maxInput, int minOutput, int maxOutput) {
    int rawValue = m_inputManager->getRawChannelValue(channel);
    return map(rawValue, minInput, maxInput, minOutput, maxOutput);
}

void HeliLogic::update(SensorHandler* sensors) {
    unsigned long currentTime = millis();
    float dt = (currentTime - lastUpdateTime) / 1000.0f;
    lastUpdateTime = currentTime;

    // Eingabekanäle auslesen und mappen
    unsigned long rollInput = mapChannelValue(m_rollChannel, 172, 1811, -500, 500);
    unsigned long pitchInput = mapChannelValue(m_pitchChannel, 172, 1811, -500, 500);
    unsigned long thrustInput = mapChannelValue(m_thrustChannel, 172, 1811, 1000, 2000);
    unsigned long yawInput = mapChannelValue(m_yawChannel, 172, 1811, 1000, 2000);
    unsigned long motorMainInput = mapChannelValue(m_motorMainChannel, 172, 1811, 1000, 2000);


    // Serial.printf("Roll: %lu, Pitch: %lu, Thrust: %lu, Yaw: %lu\n", rollInput, pitchInput, thrustInput, yawInput);


    // Hauptmotor steuern
    controlMainMotor(motorMainInput);

    // Heckmotor steuern
    unsigned long motorAftInput = calculateAftMotorInput(motorMainInput, yawInput);
    controlAftMotor(motorAftInput);

    // PID-Korrekturen und Servoansteuerung
    float roll, pitch, yaw;
    sensors->getFilteredData(roll, pitch, yaw);
    float rollCorrection = calculatePID(roll, lastRollError, dt);
    float pitchCorrection = calculatePID(pitch, lastPitchError, dt);

    unsigned long servoBackPulse, servoLeftPulse, servoRightPulse;
    calculateSwashplatePositions(rollInput, pitchInput, thrustInput, rollCorrection, pitchCorrection, servoBackPulse, servoLeftPulse, servoRightPulse);

    servoBack.writeMicroseconds(servoBackPulse);
    servoLeft.writeMicroseconds(servoLeftPulse);
    servoRight.writeMicroseconds(servoRightPulse);
}

/**
* Die PID Regelung berechnet den neuen Wert für die Servoansteuerung
* @param error Der aktuelle Fehler
* @param lastError Der Fehler des letzten Schrittes
* @param dt Die Zeitdifferenz zum letzten Schritt
* @return Summer der drei Terme
*/
float HeliLogic::calculatePID(float error, float& lastError, float dt) {
    float pTerm = kp * error;
    iTerm += ki * error * dt;
    float dTerm = kd * (error - lastError) / dt;
    lastError = error;
    return pTerm + iTerm + dTerm;
}

unsigned long HeliLogic::calculateAftMotorInput(unsigned long motorMainInput, unsigned long yawInput) {
    float baseSpeed = motorMainInput;
    float yawAdjustment = constrain(map(yawInput, 1000, 2000, -200, 200) * yawFactor, -200, 200);
    return constrain(baseSpeed + yawAdjustment, 1000, 2000);
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

    // Serial.printf("Servo Back: %lu, Left: %lu, Right: %lu\n", servoBackPulse, servoLeftPulse, servoRightPulse);

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
