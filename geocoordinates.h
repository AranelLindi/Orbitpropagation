#ifndef GEOCOORDINATES_H
#define GEOCOORDINATES_H

// Basisklasse für Koordinatenobjekte
struct BaseCoordinate {
    double latitude;
    double longitude;
    double heigth;
};

struct GeocentricCoordinate : BaseCoordinate {};
struct GeodeticCoordinate : BaseCoordinate {};

#endif // GeoCoordinates.h