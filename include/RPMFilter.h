#ifndef RPMFILTER_H
#define RPMFILTER_H

class RPMFilter {
public:
    RPMFilter();
    void setRPM(int rpm);
    float apply(float gyroValue);

private:
    float notchFrequency;
    float calculateNotchFrequency(int rpm);
    float bandwidth;
};

#endif // RPMFILTER_H
