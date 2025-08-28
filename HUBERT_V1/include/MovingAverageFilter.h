#ifndef MOVINGAVERAGEFILTER_H
#define MOVINGAVERAGEFILTER_H

class MovingAverageFilter {
public:
    // Constructor that takes the size of the moving window (N)
    MovingAverageFilter(int size);
    
    // Apply the filter to the current value
    float apply(float newValue);

private:
    int size;        // Size of the moving window
    float* values;   // Array to store the last N values
    float sum;       // Sum of the last N values
    int index;       // Index for the circular buffer
};

#endif // MOVINGAVERAGEFILTER_H
