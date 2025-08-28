#include <esp_task_wdt.h>
#include "WDT.h"

// Initialisiert den Watchdog-Timer mit einem Timeout in Sekunden
void initWatchdog(int timeoutSeconds) {
    esp_task_wdt_init(timeoutSeconds, true);  // Initialisiere den Watchdog mit Timeout und Neustart bei Timeout
    esp_task_wdt_add(NULL);  // Füge den Haupttask dem Watchdog hinzu
}

// Setzt den Watchdog-Timer zurück, um einen Neustart zu vermeiden
void resetWatchdog() {
    esp_task_wdt_reset();  // Setze den Watchdog-Timer zurück
}
