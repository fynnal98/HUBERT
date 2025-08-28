#ifndef PID_H
#define PID_H

class PID {
public:
    PID(float kp, float ki, float kd, float integralLimit, float factor);
    float compute(float setpoint, float measured);
    void setSetpoint(float setpoint); // Hinzugefügte Methode zum Setzen des Sollwerts

    // PID-Parameter öffentlich machen
    float kp, ki, kd;

private:
    float prevError;
    float integral;
    float integralLimit;
    float factor;
};

#endif // PID_H