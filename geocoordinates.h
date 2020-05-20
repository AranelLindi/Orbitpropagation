#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

#include "SGP4Propagator/SGP4Propagator.h"

// Basisklasse für Koordinatenobjekte
struct BaseCoordinate {
    double latitude;
    double longitude;
    double heigth;
};

struct GeocentricCoordinate : BaseCoordinate {};
struct GeodeticCoordinate : BaseCoordinate {};

GeocentricCoordinate convertECItoGeocentric(const ECICoordinate& eciCoord, double jd);

GeodeticCoordinate convertECItoGeodetic(const ECICoordinate& eciCoord, double jd);


#endif // GeoCoordinates.h