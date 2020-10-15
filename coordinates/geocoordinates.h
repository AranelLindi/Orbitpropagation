#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

#include "../SGP4Propagator/SGP4Propagator.h"
#include "../calendars/calendar.h"

// Basisklasse für Koordinatenobjekte
struct BaseCoordinate
{
    const double latitude;
    const double longitude;
    const double heigth;

    BaseCoordinate(double _latitude, double _longitude, double _heigth) : latitude(_latitude), longitude(_longitude), heigth(_heigth) {}

    void print(void) const;
};

struct GeocentricCoordinate : BaseCoordinate
{

    GeocentricCoordinate(double _latitude, double _longitude, double _heigth) : BaseCoordinate(_latitude, _longitude, _heigth) {}

    void print(void) const;
};
struct GeodeticCoordinate : BaseCoordinate
{
    GeodeticCoordinate(double _latitude, double _longitude, double _heigth) : BaseCoordinate(_latitude, _longitude, _heigth) {}

    void print(void) const;
};

struct CoordinateConvertion
{
    // Globale Konstante, wird in beiden Funktionen benötigt:
    const float Re{6378.137f}; // [km] Radius der Erde

    GeocentricCoordinate convertECItoGeocentric(const ECICoordinate &eciCoord, double jd);

    GeodeticCoordinate convertECItoGeodetic(const ECICoordinate &eciCoord, double jd);
};

#endif // GeoCoordinates.h