#ifndef LOWPASSFILTER_H
#define LOWPASSFILTER_H

class LowPassFilter {
public:
    // Konstruktor mit Grenzfrequenz und Abtastrate
    LowPassFilter(float cutoffFrequency, float sampleRate);
    
    // Methode, um Grenzfrequenz und Abtastrate zu setzen
    void setParameters(float cutoffFrequency, float sampleRate);
    
    // Methode, um den Filter auf den aktuellen Wert anzuwenden
    float apply(float currentValue);

private:
    float alpha;           // Der berechnete Alpha-Wert für den Filter
    float previousValue;    // Der vorherige gefilterte Wert
};

#endif
