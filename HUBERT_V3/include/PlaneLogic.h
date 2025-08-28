#ifndef PLANE_LOGIC_H
#define PLANE_LOGIC_H

#include "JsonHandler.h"
#include "InputManager.h"

class PlaneLogic {
public:
    PlaneLogic(JsonHandler* jsonHandler, InputManager* inputManager);
    void loadParameters();
    void update();  // Optionale Erweiterung später

private:
    JsonHandler* m_jsonHandler;
    InputManager* m_inputManager;
};

#endif  // PLANE_LOGIC_H
