#ifndef RPMFILTER_H
#define RPMFILTER_H

class RPMFilter {
public:
    RPMFilter();
    void setRPM(int rpm);
    void setBandwidth(float bw); 
    float apply(float gyroValue);

private:
    float notchFrequency;
    float calculateNotchFrequency(int rpm);
};


#endif // RPMFILTER_H
