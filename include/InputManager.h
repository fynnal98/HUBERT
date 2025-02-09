#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <vector>
#include <string>

class JsonHandler;
class SBUSHandler;
class UDPManager;

class InputManager {
public:
    InputManager(JsonHandler* db, SBUSHandler* sbusHandler, UDPManager* udpManager);
    void begin();
    void update();
    int getRawChannelValue(int channel);

private:
    JsonHandler* m_db;
    SBUSHandler* m_sbusHandler;
    UDPManager* m_udpManager;
    std::string m_inputMethod;
    std::vector<int> m_channelValues;
};

#endif // INPUT_MANAGER_H
