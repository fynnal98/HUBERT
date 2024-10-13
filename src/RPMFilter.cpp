#include "RPMFilter.h"
#include <cmath>

RPMFilter::RPMFilter() : notchFrequency(0.0), bandwidth(10.0) {}

void RPMFilter::setRPM(int rpm) {
    notchFrequency = calculateNotchFrequency(rpm);
}

float RPMFilter::calculateNotchFrequency(int rpm) {
    return rpm / 60.0; // Convert RPM to Hz
}

float RPMFilter::apply(float gyroValue) {
    // Placeholder for actual notch filter implementation
    // If the gyro value is within the notch frequency range, attenuate it
    if (std::abs(gyroValue) > notchFrequency - bandwidth / 2 && std::abs(gyroValue) < notchFrequency + bandwidth / 2) {
        return gyroValue * 0.5; // Attenuate the resonance frequency
    }
    return gyroValue;
}
