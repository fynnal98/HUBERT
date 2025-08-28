#ifndef HIGHPASSFILTER_H
#define HIGHPASSFILTER_H

class HighPassFilter {
public:
    // Korrigierter Konstruktor mit Grenzfrequenz und Abtastrate
    HighPassFilter(float cutoffFrequency, float sampleRate);
    
    // Methode zum Anwenden des Filters
    float apply(float currentValue);

private:
    float alpha;
    float previousValue;
    float previousInput;  // Hinzufügen der fehlenden Variable für den vorherigen Eingang
};

#endif
