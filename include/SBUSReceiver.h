#ifndef SBUSRECEIVER_H
#define SBUSRECEIVER_H

#include <Arduino.h>
#include <SBUS.h>

class SBUSReceiver {
public:
    SBUSReceiver(HardwareSerial& serialPort);
    void begin();
    bool readChannels(unsigned int& channel1, unsigned int& channel2, unsigned int& channel3, unsigned int& channel4, unsigned int& channel6, unsigned int& channel8, unsigned int& channel9, unsigned int& channel10);
    
    // Neue Methode zum Abrufen des Werts von Kanal 10
    unsigned int getChannel10Pulse();

private:
    bfs::SbusRx sbus_rx;
    bfs::SbusData data;
};

#endif
