#ifndef TASK2_2_H
#define TASK2_2_H

// eigener Code
#include "GeoCoordinates.h"
#include "../SGP4Propagator/SGP4Propagator.cpp"


double computeJD(int year, double dayFractions); // Berechnet Julianisches Datum mit Jahr und Tagesbruchteil

double computeGMST(double jd);

GeocentricCoordinate convertECItoGeocentric(const ECICoordinate& eciCoord, double jd);

GeodeticCoordinate convertECItoGeodetic(const ECICoordinate& eciCoord, double jd);


#endif // task2_2.h