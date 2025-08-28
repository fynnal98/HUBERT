#include "MPU6050.h"

void MPU6050::init() {
    Wire.begin();
    writeRegister(0x6B, 0x00);  // Weckt den MPU6050 auf (PWR_MGMT_1 = 0)
}

void MPU6050::readAccel(float& x, float& y, float& z) {
    uint8_t buffer[6];
    readRegisters(0x3B, buffer, 6);  // Beschleunigungsdaten ab 0x3B auslesen

    x = ((int16_t)(buffer[0] << 8 | buffer[1])) / 16384.0;  // Normierung auf g
    y = ((int16_t)(buffer[2] << 8 | buffer[3])) / 16384.0;
    z = ((int16_t)(buffer[4] << 8 | buffer[5])) / 16384.0;
}

void MPU6050::readGyro(float& x, float& y, float& z) {
    uint8_t buffer[6];
    readRegisters(0x43, buffer, 6);  // Gyroskopdaten ab 0x43 auslesen

    x = ((int16_t)(buffer[0] << 8 | buffer[1])) / 131.0;  // Normierung auf °/s
    y = ((int16_t)(buffer[2] << 8 | buffer[3])) / 131.0;
    z = ((int16_t)(buffer[4] << 8 | buffer[5])) / 131.0;
}

void MPU6050::writeRegister(uint8_t reg, uint8_t value) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.write(value);
    Wire.endTransmission();
}

void MPU6050::readRegisters(uint8_t reg, uint8_t* buffer, uint8_t length) {
    Wire.beginTransmission(MPU_ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_ADDR, length);

    for (uint8_t i = 0; i < length; i++) {
        buffer[i] = Wire.read();
    }
}
