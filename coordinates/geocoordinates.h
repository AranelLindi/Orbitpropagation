#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

#include "../SGP4Propagator/SGP4Propagator.h"
#include "../calendars/calendar.h"

// Basisklasse für Koordinatenobjekte
struct BaseCoordinate
{
    const double latitude;
    const double longitude;
    const double height;

    BaseCoordinate(double _latitude, double _longitude, double _height) : latitude(_latitude), longitude(_longitude), height(_height) {}

    void print(void) const noexcept;
};

struct GeocentricCoordinate : BaseCoordinate
{
    GeocentricCoordinate(double _latitude, double _longitude, double _height) : BaseCoordinate(_latitude, _longitude, _height) {}

    void print(void) const noexcept;
};
struct GeodeticCoordinate : BaseCoordinate
{
    GeodeticCoordinate(double _latitude, double _longitude, double _height) : BaseCoordinate(_latitude, _longitude, _height) {}

    void print(void) const noexcept;
};

struct CoordinateConvertion
{
    // Globale Konstante, wird in beiden Funktionen benötigt:
    const float Re{6378.137f}; // [km] Radius der Erde

    GeocentricCoordinate convertECItoGeocentric(const ECICoordinate &eciCoord, double jd) noexcept;

    GeodeticCoordinate convertECItoGeodetic(const ECICoordinate &eciCoord, double jd) noexcept;
};

#endif // GeoCoordinates.h