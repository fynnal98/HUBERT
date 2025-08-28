#ifndef WDT_H
#define WDT_H

// Funktion zur Initialisierung des Watchdog-Timers
void initWatchdog(int timeoutSeconds);

// Funktion zum Zurücksetzen des Watchdog-Timers
void resetWatchdog();

#endif // WDT_H
