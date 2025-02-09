#ifndef DATAGRAMHANDLER_H
#define DATAGRAMHANDLER_H

#include <vector>
#include <string>
#include <cstdint>
#include <ArduinoJson.h>

class DatagramHandler {
public:
    std::vector<uint8_t> createDatagram(const std::string& group, const std::string& jsonData);
    void parseDatagram(const std::vector<uint8_t>& datagram);

private:
    uint8_t calculateChecksum(const std::vector<uint8_t>& data);
    std::string findGroupByPayloadType(uint8_t payloadType);
    void decodePayload(const std::string& groupKey, const std::vector<uint8_t>& payload);
};

#endif
