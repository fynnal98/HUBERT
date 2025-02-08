#ifndef DATAGRAMHANDLER_H
#define DATAGRAMHANDLER_H

#include <vector> 
#include <string>
#include <cstdint> 

class DatagramHandler {
    public: 
        std::vector<uint8_t> createDatagram(const std::string& msgType, const std::vector<uint8_t>& payload);
};

#endif 
