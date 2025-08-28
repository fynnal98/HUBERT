#include "SBUSInput.h"

SBUSInput::SBUSInput(HardwareSerial* serial, int rxPin, int txPin)
    : sbus(serial, rxPin, txPin, true) {}

void SBUSInput::init(DatabaseTool* db) {
    sbus.Begin();
    Serial.println("SBUSInput gestartet.");
}

void SBUSInput::update() {
    if (sbus.Read()) {
        auto data = sbus.data();
        receivedData.clear();
        for (int i = 0; i < bfs::SbusData::NUM_CH; ++i) {
            receivedData += std::to_string(data.ch[i]) + ",";
        }
    }
}

std::string SBUSInput::getInput() {
    return receivedData;
}
