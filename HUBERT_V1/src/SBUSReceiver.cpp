#include "SBUSReceiver.h"

SBUSReceiver::SBUSReceiver(HardwareSerial& serialPort)
    : sbus_rx(&serialPort, 16, -1, true) {}

void SBUSReceiver::begin() {
    sbus_rx.Begin();
}

bool SBUSReceiver::readChannels(unsigned int& channel1, unsigned int& channel2, unsigned int& channel3, unsigned int& channel4, unsigned int& channel6, unsigned int& channel8, unsigned int& channel9, unsigned int& channel10) {
    if (sbus_rx.Read()) {
        data = sbus_rx.data();

        // Mappe die SBUS-Kanalwerte zu Servo-Pulsweiten (1000-2000 us)
        channel1 = map(data.ch[0], 172, 1811, 1000, 2000);
        channel2 = map(data.ch[1], 172, 1811, 1000, 2000);
        channel3 = map(data.ch[2], 172, 1811, 1000, 2000);
        channel4 = map(data.ch[3], 172, 1811, 1000, 2000); 
        channel6 = map(data.ch[5], 172, 1811, 1000, 2000);
        channel8 = map(data.ch[7], 172, 1811, 1000, 2000);
        channel9 = map(data.ch[8], 172, 1811, 1000, 2000);
        channel10 = map(data.ch[9], 172, 1811, 1000, 2000);

        return true;
    }
    return false;
}
