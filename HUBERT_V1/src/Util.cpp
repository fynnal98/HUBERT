#include "Util.h"

bool Util::correctionEnabled(unsigned int channel10Pulse) {
    int toggleThreshold = 1500;
    return channel10Pulse > toggleThreshold;
}