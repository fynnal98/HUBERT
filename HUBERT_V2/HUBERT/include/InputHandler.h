#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "WiFiInput.h"  // Statt Vorwärtsdeklaration direkt den Header einbinden
#include "SBUSInput.h"
#include <map>
#include <string>
#include <vector>

class InputHandler {
public:
    static InputHandler* createHandler(DatabaseTool* db);
    void init(DatabaseTool* db);
    void update();  // Verarbeitet entweder SBUS oder App-Steuerdaten
    void updateWithAppData(const std::string& rawData);  // Verarbeitet Steuerdaten direkt aus der App
    std::string getInput();  // Gibt empfangene Daten zurück
    int getChannelValue(const std::string& channel);

private:
    std::map<std::string, int> channelMapping;
    std::map<std::string, int> channelValues;
    WiFiInput* wifiInput = nullptr;
    SBUSInput* sbusInput = nullptr;

    void processSBUSInput();  // SBUS-Daten verarbeiten
};

#endif  // INPUTHANDLER_H
