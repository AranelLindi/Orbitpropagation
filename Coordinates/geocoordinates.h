#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

#include "SGP4Propagator/SGP4Propagator.h"
#include "calendar.h"

// Basisklasse für Koordinatenobjekte
struct BaseCoordinate
{
    double latitude;
    double longitude;
    double heigth;

    void print(void);
};

struct GeocentricCoordinate : BaseCoordinate
{
    void print(void);
};
struct GeodeticCoordinate : BaseCoordinate
{
    void print(void);
};

struct CoordinateConvertion
{
    // Globale Konstante, wird in beiden Funktionen benötigt:
    static const float Re = 6378.137; // [km] Radius der Erde
    

    GeocentricCoordinate convertECItoGeocentric(const ECICoordinate &eciCoord, double jd);

    GeodeticCoordinate convertECItoGeodetic(const ECICoordinate &eciCoord, double jd);
};

#endif // GeoCoordinates.h