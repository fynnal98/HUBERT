#include "DatagramHandler.h"

std::vector<uint8_t> DatagramHandler::createDatagram(const std::string& msgType, const std::vector<uint8_t>& payload) {
    
    std::vector<uint8_t> datagram; //Leerer Vektor für das Datagramm
    
    datagram.push_back(0x20); // Startbyte 

    if (msgType == "config") {
        datagram.push_back(0x01);
    } else if (msgType == "telemetry") {
        datagram.push_back(0x02);
    } else {
        datagram.push_back(0xff); // unbekannter typ 
    }

    datagram.push_back(0x00); //msg Cnter 

    datagram.push_back(payload.size()); // Länge des Payloads

    datagram.insert(datagram.end(), payload.begin(), payload.end()); // Payload anhängen

    uint8_t checksum = 0; // Checksumme berechnen
    for (const auto& byte : datagram) {
        checksum += byte; 
    }
    datagram.push_back(checksum); // Checksumme anhängen

    datagram.push_back(0x30); // Endbyte

    return datagram;

    