#include "ParameterUpdater.h"
#include <Arduino.h>

void ParameterUpdater::init(DatabaseTool* db) {
    this->database = db;
}

void ParameterUpdater::updateParameter(const std::string& data) {
    size_t delimiterPos = data.find('=');
    if (delimiterPos != std::string::npos) {
        std::string parameterPath = data.substr(0, delimiterPos);
        std::string value = data.substr(delimiterPos + 1);

        if (value.find('.') != std::string::npos) {
            database->set(parameterPath.c_str(), std::stof(value));
        } else {
            database->set(parameterPath.c_str(), std::stoi(value));
        }
        Serial.printf("Parameter %s erfolgreich auf %s gesetzt.\n", parameterPath.c_str(), value.c_str());
    } else {
        Serial.println("Ungültiger Parameterbefehl empfangen.");
    }
}
