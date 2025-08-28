#include <Arduino.h>
#include "WiFiInput.h"
#include "DatabaseTool.h"
#include "InputHandler.h"
#include "SensorHandler.h"
#include "ModeManager.h"

DatabaseTool database;
WiFiInput wifiInput;
SensorHandler sensorHandler;
InputHandler* inputHandler;
ModeManager* modeManager;

void setup() {
    Serial.begin(115200);
    
    database.init();
    
    inputHandler = InputHandler::createHandler(&database);
    
    sensorHandler.begin(&database);
    
    wifiInput.init(&database, inputHandler);
    
    modeManager = new ModeManager(&database);
    modeManager->init();
}

void loop() {
    inputHandler->update();
    
    wifiInput.update();
    
    sensorHandler.update();
    
    modeManager->update(&sensorHandler, inputHandler);
    
    delay(10);
}
