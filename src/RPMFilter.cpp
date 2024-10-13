#include "RPMFilter.h"
#include "ParameterHandler.h"
#include <cmath>

RPMFilter::RPMFilter() : notchFrequency(0.0) {}

void RPMFilter::setRPM(int rpm) {
    notchFrequency = calculateNotchFrequency(rpm);
}

void RPMFilter::setBandwidth(float bw) {
    bandwidth = bw;  // Bandwidth wird nun gesetzt
}

float RPMFilter::calculateNotchFrequency(int rpm) {
    return rpm / 60.0; // Convert RPM to Hz
}

float RPMFilter::apply(float gyroValue) {
    // RPM-Filter basierend auf notchFrequency und bandwidth
    if (std::abs(gyroValue) > notchFrequency - bandwidth / 2 && std::abs(gyroValue) < notchFrequency + bandwidth / 2) {
        return gyroValue * 0.5;
    }
    return gyroValue;
}
