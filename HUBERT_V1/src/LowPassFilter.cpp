#include "LowPassFilter.h"
#include <cmath>  // Für M_PI

LowPassFilter::LowPassFilter(float cutoffFrequency, float sampleRate)
    : previousValue(0.0) {
    // Berechne Alpha basierend auf Grenzfrequenz und Abtastrate
    alpha = 2 * M_PI * cutoffFrequency / (2 * M_PI * cutoffFrequency + sampleRate);
}

float LowPassFilter::apply(float currentValue) {
    // Filterformel: y[i] = α * x[i] + (1 - α) * y[i-1]
    float filteredValue = alpha * currentValue + (1.0 - alpha) * previousValue;
    previousValue = filteredValue;
    return filteredValue;
}
