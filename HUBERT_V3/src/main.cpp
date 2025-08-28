#include <Arduino.h>
#include <SPIFFS.h>

#include "JsonHandler.h"
#include "WiFiHandler.h"
#include "SBUSHandler.h"
#include "UDPManager.h"
#include "InputManager.h"
#include "SensorHandler.h"
#include "ModeManager.h"

JsonHandler jsonHandler("/config.json");
WiFiHandler wifiHandler;
SBUSHandler sbusHandler;
UDPManager udpManager;  
InputManager inputManager(&jsonHandler, &sbusHandler, &udpManager);
SensorHandler sensorHandler;
ModeManager modeManager(&jsonHandler, &inputManager);


void setup() {
    Serial.begin(115200);
    SPIFFS.begin(true);
    jsonHandler.LoadJson();
    sbusHandler.begin();
    wifiHandler.begin();
    inputManager.begin();
    sensorHandler.begin(&jsonHandler);
    modeManager.begin();
}

void loop() {
    inputManager.update();
    sensorHandler.update();
    sbusHandler.update(); 
    wifiHandler.handleClients();
    modeManager.update(&sensorHandler); 
    delay (10);
}
