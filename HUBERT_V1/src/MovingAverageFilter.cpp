#include "MovingAverageFilter.h"
#include <stdlib.h>

// Constructor to initialize the filter with a given window size (N)
MovingAverageFilter::MovingAverageFilter(int size) 
    : size(size), sum(0), index(0) {
    values = new float[size]();  // Allocate memory for N values
}

float MovingAverageFilter::apply(float newValue) {
    // Subtract the oldest value from the sum
    sum -= values[index];
    
    // Store the new value in the circular buffer
    values[index] = newValue;
    
    // Add the new value to the sum
    sum += newValue;
    
    // Move the index forward, wrap around if necessary
    index = (index + 1) % size;
    
    // Return the average of the last N values
    return sum / size;
}
