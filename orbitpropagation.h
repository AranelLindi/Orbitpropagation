#ifndef ORBITPROPAGATION_H
#define ORBITPROPAGATION_H

/*
* Header der included werden muss um das Projekt einzubinden
* 
*/

// eigener Code
#include "geocoordinates.h"                // für Koordinatenumrechnungen und -Container
#include "SGP4Propagator/SGP4Propagator.h" // berechnet Position und Geschwindigkeit

#include "calendar.h"                      // bietet verschiedene Funktionen für Kalenderberechnungen
#include "TLEinterpreter/tleinterpreter.h" // liest ein gegebenes File mit TLE Datensätzen ein

#endif // orbitpropagation.h