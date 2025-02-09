#ifndef HELI_LOGIC_H
#define HELI_LOGIC_H

#include "JsonHandler.h"
#include "InputManager.h"
#include "SensorHandler.h"
#include <ESP32Servo.h>

class HeliLogic {
public:
    HeliLogic(JsonHandler* jsonHandler, InputManager* inputManager);
    void loadParameters();
    void update(SensorHandler* sensors);

private:
    JsonHandler* m_jsonHandler;
    InputManager* m_inputManager;

    int m_rollChannel, m_pitchChannel, m_thrustChannel, m_yawChannel, m_motorMainChannel;

    int m_motorMainPin, m_motorAftPin, m_servoBackPin, m_servoLeftPin, m_servoRightPin;
    Servo mainMotor, aftMotor, servoBack, servoLeft, servoRight;

    float kp, ki, kd, yawFactor;
    float iTerm = 0;
    float lastRollError = 0, lastPitchError = 0;
    unsigned long lastUpdateTime = 0;

    unsigned long mapChannelValue(int channel, int minInput = 172, int maxInput = 1811, int minOutput = 1000, int maxOutput = 2000);
    float calculatePID(float error, float& lastError, float dt);
    unsigned long calculateAftMotorInput(unsigned long motorMainInput, unsigned long yawInput);
    void calculateSwashplatePositions(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput, float rollCorrection, float pitchCorrection,
                                      unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void applyInputs(unsigned long rollInput, unsigned long pitchInput, unsigned long thrustInput, unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void applyCorrections(float rollCorrection, float pitchCorrection, unsigned long& servoBackPulse, unsigned long& servoLeftPulse, unsigned long& servoRightPulse);
    void controlMainMotor(unsigned long motorMainInput);
    void controlAftMotor(unsigned long motorAftInput);
};

#endif  // HELI_LOGIC_H
