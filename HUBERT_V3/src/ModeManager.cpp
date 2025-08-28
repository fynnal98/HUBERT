#include "ModeManager.h"
#include "HeliLogic.h"
#include "DroneLogic.h"
#include "PlaneLogic.h"
#include <Arduino.h>

ModeManager::ModeManager(JsonHandler* jsonHandler, InputManager* inputManager)
    : m_jsonHandler(jsonHandler), m_inputManager(inputManager), m_heliLogic(nullptr), m_droneLogic(nullptr), m_planeLogic(nullptr) {}

void ModeManager::begin() {
    // Lade den initialen Modus aus der JSON-Konfiguration
    m_currentMode = m_jsonHandler->GetValueByKey<std::string>("config/system/mode", "heli");
    Serial.printf("Initialer Modus: %s\n", m_currentMode.c_str());

    // Initialisiere die zugehörigen Logiken
    m_heliLogic = new HeliLogic(m_jsonHandler, m_inputManager);
    m_droneLogic = new DroneLogic(m_jsonHandler, m_inputManager);
    m_planeLogic = new PlaneLogic(m_jsonHandler, m_inputManager);

    // Lade die Parameter für den aktuellen Modus
    loadModeParameters(m_currentMode);
}

void ModeManager::switchMode(const std::string& mode) {
    if (mode != m_currentMode) {
        Serial.printf("Wechsle von Modus %s zu Modus %s\n", m_currentMode.c_str(), mode.c_str());
        m_currentMode = mode;
        loadModeParameters(mode);
    }
}

void ModeManager::update(SensorHandler* sensors) {
    if (m_currentMode == "heli") {
        m_heliLogic->update(sensors);
    } else if (m_currentMode == "drone") {
        m_droneLogic->update();
    } else if (m_currentMode == "plane") {
        m_planeLogic->update();
    }
}

void ModeManager::loadModeParameters(const std::string& mode) {
    Serial.printf("Lade Parameter für Modus: %s\n", mode.c_str());

    if (mode == "heli") {
        m_heliLogic->loadParameters();
    } else if (mode == "drone") {
        m_droneLogic->loadParameters();
    } else if (mode == "plane") {
        m_planeLogic->loadParameters();
    }
}
