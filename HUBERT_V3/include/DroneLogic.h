#ifndef DRONE_LOGIC_H
#define DRONE_LOGIC_H

#include "JsonHandler.h"
#include "InputManager.h"

class DroneLogic {
public:
    DroneLogic(JsonHandler* jsonHandler, InputManager* inputManager);
    void loadParameters();
    void update();  // Optionale Erweiterung später

private:
    JsonHandler* m_jsonHandler;
    InputManager* m_inputManager;
};

#endif  // DRONE_LOGIC_H
