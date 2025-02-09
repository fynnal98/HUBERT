#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H

#include <string>
#include "JsonHandler.h"
#include "InputManager.h"
#include "SensorHandler.h"

class HeliLogic;
class DroneLogic;
class PlaneLogic;

class ModeManager {
public:
    ModeManager(JsonHandler* jsonHandler, InputManager* inputManager);

    void begin();  // Initialisiert den ModeManager
    void switchMode(const std::string& mode);  // Wechselt zur Laufzeit den Modus
    void update(SensorHandler* sensors);  // Führt die Steuerlogik des aktuellen Modus aus

private:
    JsonHandler* m_jsonHandler;
    InputManager* m_inputManager;
    std::string m_currentMode;
    HeliLogic* m_heliLogic;
    DroneLogic* m_droneLogic;
    PlaneLogic* m_planeLogic;

    void loadModeParameters(const std::string& mode);
};

#endif  // MODE_MANAGER_H
