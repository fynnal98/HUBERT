#include "HighPassFilter.h"
#include <cmath>  // Für M_PI

// Konstruktor: Berechne Alpha basierend auf Grenzfrequenz und Abtastrate
HighPassFilter::HighPassFilter(float cutoffFrequency, float sampleRate)
    : previousValue(0.0), previousInput(0.0) {  // Initialisiere previousInput
    // Berechne Alpha
    alpha = 2 * M_PI * cutoffFrequency / (2 * M_PI * cutoffFrequency + sampleRate);
}

float HighPassFilter::apply(float currentValue) {
    // High-pass filter formula: y[i] = α * (y[i-1] + x[i] - x[i-1])
    float filteredValue = alpha * (previousValue + currentValue - previousInput);
    previousValue = filteredValue;
    previousInput = currentValue;
    return filteredValue;
}
