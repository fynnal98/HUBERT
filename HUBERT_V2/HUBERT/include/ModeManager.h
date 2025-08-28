#ifndef MODEMANAGER_H
#define MODEMANAGER_H

#include <string>
#include "DatabaseTool.h"
#include "DroneLogic.h"
#include "PlaneLogic.h"
#include "HeliLogic.h"
#include "SensorHandler.h"
#include "InputHandler.h"

class ModeManager {
public:
    ModeManager(DatabaseTool* db);
    void init();
    void update(SensorHandler* sensors, InputHandler* input);

private:
    DatabaseTool* database;
    std::string currentMode;

    DroneLogic* droneLogic;
    PlaneLogic* planeLogic;
    HeliLogic* heliLogic;

    void setupDroneMode();
    void setupPlaneMode();
    void setupHeliMode();

    void cleanupDroneMode();
    void cleanupPlaneMode();
    void cleanupHeliMode();
};

#endif
