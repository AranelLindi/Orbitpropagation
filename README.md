# Orbitpropagation
Verwendet das SGP4 Bahnmodell um den Orbit von Satelliten vorherzusagen. Ausgangspunkt sind dabei TLEs (Two Lines Elements). Als Ergebnis liefert SGP4 die Position und Geschwindigkeit eines Satelliten im ECI System.

## **calendars/** calendar.h/cpp ##
Stellt Kalendaroperationen bereit. Unter anderem: **Umwandlung eines Datums aus dem gregorianischen Kalender in den julianischen**, entweder über eine *DayFraction* oder über ein genaues *Datum*. Des Weiteren auch Funktionen die auf ein **Schaltjahr testen**. Ebenso eine Funktion, die ausgehend von einer Julian Day Number die **GMST *(Greenwich Mean Sidereal Time) berechnet***.

## **coordinates/** geocordinates.h/cpp ##
Stellt Container für verschiedene Koordinatentypen bereit (**GeocentricCoordinate** und **GeodeticCoordinate**) und bietet auch Funktionen, die in Abhängigkeit einer *Julian Day Number (JDN)* die Koordinatensystem ineinander umrechnen.

## **SGP4Propagator/** SGP4Propagator.h/cpp ##
Bietet einen **ECICoordinate**-Container für die Darstellung von Koordinaten an, sowie in Verbindung mit einem TLE Reader / Interpreter eine Funktion, die abhängig von der Epoche die Position und Geschwindigkeit eines Satelliten mithilfe eines TLE Datensatzes berechnet.

## main.cpp ##
Muss nicht in neue Projekte eingebunden werden, bietet aber eine beispielhafte Darstellung der Implementierung.


-----------------
Stefan Lindörfer
 02. Juni 2020
