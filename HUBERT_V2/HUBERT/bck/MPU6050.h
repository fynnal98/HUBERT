#ifndef MPU6050_H
#define MPU6050_H

#include <Wire.h>

class MPU6050 {
public:
    void init();  // Initialisiert den Sensor
    void readAccel(float& x, float& y, float& z);  // Liest Beschleunigungswerte aus
    void readGyro(float& x, float& y, float& z);   // Liest Gyroskopwerte aus

private:
    void writeRegister(uint8_t reg, uint8_t value);
    void readRegisters(uint8_t reg, uint8_t* buffer, uint8_t length);

    const uint8_t MPU_ADDR = 0x68;  // Standard I2C-Adresse des MPU6050
};

#endif
