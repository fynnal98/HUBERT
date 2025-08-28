#ifndef HELI_LOGIC_H
#define HELI_LOGIC_H

#include <ESP32Servo.h>
#include "DatabaseTool.h"
#include "SensorHandler.h"
#include "InputHandler.h"

class HeliLogic {
public:
    HeliLogic(DatabaseTool* db);
    void init();
    void update(SensorHandler* sensors, InputHandler* input);

private:
    DatabaseTool* database;
    unsigned long lastUpdateTime;

    Servo mainMotor;
    Servo aftMotor;
    Servo servoBack;
    Servo servoLeft;
    Servo servoRight;

    int motorMainPin;
    int motorAftPin;
    int servoBackPin;
    int servoLeftPin;
    int servoRightPin;

    float kp, ki, kd;
    float yawFactor;
    float iTerm;
    float lastRollError;
    float lastPitchError;

    float calculatePID(float error, float& lastError, float dt);
    unsigned long calculateAftMotorInput(unsigned long motorMainInput, unsigned long yawInput);
    void calculateSwashplatePositions(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                                       float rollCorrection, float pitchCorrection,
                                       unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void applyInputs(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput,
                     unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void applyCorrections(float rollCorrection, float pitchCorrection,
                          unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void controlMainMotor(unsigned long motorMainInput);
    void controlAftMotor(unsigned long motorAftInput);
};

#endif
