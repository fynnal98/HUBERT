#ifndef SERIALHANDLER_H
#define SERIALHANDLER_H

#include <Arduino.h>  // Include für den String-Typ

// Funktion zum Verarbeiten von seriellen Daten
void processSerialData();

// Hilfsfunktion zum Extrahieren von Parameterwerten
float extractParameterValue(const String& input, const String& parameterName);

#endif // SERIALHANDLER_H
