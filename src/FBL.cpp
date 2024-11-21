#include "FBL.h"
#include "ParameterHandler.h"  // Importiere ParameterHandler

// Constructor
FBL::FBL(int pin1, int pin2, int pin3, FilterHandler& rollFilterHandler, FilterHandler& pitchFilterHandler, DataLogger& logger)
    : servo1Pin(pin1), servo2Pin(pin2), servo3Pin(pin3),
      rollFilterHandler(rollFilterHandler), pitchFilterHandler(pitchFilterHandler),
      logger(logger)  // Logger hinzufügen
{}

void FBL::setup() {
    servo1.attach(servo1Pin); // Back
    servo2.attach(servo2Pin); // Left
    servo3.attach(servo3Pin); // Right
}

void FBL::update(MPU6050& mpu, unsigned long channel1Pulse, unsigned long channel2Pulse, unsigned long channel6Pulse,
                 bool useLowPass, bool useHighPass, bool useMovingAvg, bool useKalman) {
    // Fetch sensor data
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Corrected accelerations now handled inside MPU6050
    float ax_corrected, ay_corrected;
    mpu.calculateCorrectedAccelerations(&a, &g, ax_corrected, ay_corrected);

    // Apply filters but swap x (roll) and y (pitch) in the correction logic with negation for 180-degree rotation
    float pitchCorrection = rollFilterHandler.apply(-ax_corrected, useLowPass, useHighPass, useMovingAvg, useKalman, logger); // Verwende useRPMFilter aus ParameterHandler
    float rollCorrection = pitchFilterHandler.apply(-ay_corrected, useLowPass, useHighPass, useMovingAvg, useKalman, logger);  // Verwende useRPMFilter aus ParameterHandler

    // float pitchCorrection = rollFilterHandler.apply(g.gyro.x, useLowPass, useHighPass, useMovingAvg, useKalman, useRPMFilter, logger); // Verwende useRPMFilter aus ParameterHandler
    // float rollCorrection = pitchFilterHandler.apply(g.gyro.y, useLowPass, useHighPass, useMovingAvg, useKalman, useRPMFilter, logger);  // Verwende useRPMFilter aus ParameterHandler


    // Add corrections to the channel data
    unsigned long servo1Pulse = channel2Pulse + pitchCorrection; // Back
    unsigned long servo2Pulse = channel6Pulse + (-0.5 * pitchCorrection - 0.866 * rollCorrection); // Left
    unsigned long servo3Pulse = channel1Pulse - (-0.5 * pitchCorrection + 0.866 * rollCorrection); // Right

    // Set the pulse lengths for the servos
    servo1.writeMicroseconds(servo1Pulse); // Back
    servo2.writeMicroseconds(servo2Pulse); // Left
    servo3.writeMicroseconds(servo3Pulse); // Right
}
