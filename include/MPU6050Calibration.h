#ifndef MPU6050CALIBRATION_H
#define MPU6050CALIBRATION_H

#include "MPU6050.h"
#include "ParameterHandler.h"
#include <Arduino.h>

class MPU6050Calibration {
public:
    static void beginCalibration(int numSamples);
    static bool updateCalibration(MPU6050& mpu, float& gyroXOffset, float& gyroYOffset, float& gyroZOffset);
    static void resetCalibration();

private:
    static int calibrationStep;
    static int totalSamples;
    static float gyroXSum, gyroYSum, gyroZSum;
};

#endif // MPU6050CALIBRATION_H
