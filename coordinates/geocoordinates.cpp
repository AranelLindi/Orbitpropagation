#include "geocoordinates.h"

// Print-Funktion der Koordinaten-Basisklasse:
// (wird von beiden Objekten jeweils in der eigenen print-Funktion aufgerufen)
void BaseCoordinate::print(void) const
{
    std::cout << '\t' << "latitude:" << '\t' << this->latitude << '\n'
              << "\t\t\t"
              << "longitude:" << '\t' << this->longitude << '\n'
              << "\t\t\t"
              << "height:" << '\t' << this->heigth << std::endl;
}

// Ausgabe Geodetische Koordinate:
void GeodeticCoordinate::print(void) const
{
    std::cout << "GeodeticCoordinate:";
    BaseCoordinate::print();
}

// Ausgabe Geozentrische Koordinate:
void GeocentricCoordinate::print(void) const
{
    std::cout << "GeocentricCoordinate:";
    BaseCoordinate::print();
}

GeocentricCoordinate CoordinateConvertion::convertECItoGeocentric(const ECICoordinate &eciCoord, double jd)
{
    const double xsquare{eciCoord.x * eciCoord.x};
    const double ysquare{eciCoord.y * eciCoord.y};
    const double zsquare{eciCoord.z * eciCoord.z};

    const double latitude{atan(eciCoord.z / sqrt(xsquare + ysquare))}; // Nenner > 0
    double longitude{atan2(eciCoord.y, eciCoord.x) - Calendar::computeGMST(jd)};

    // Intervallprüfung longitude zwischen (-PI; PI]
    if (longitude > M_PI)
        longitude -= 2 * M_PI;
    if (longitude <= -M_PI)
        longitude += 2 * M_PI;

    const double heigth{sqrt(xsquare + ysquare + zsquare) - Re};

    return GeocentricCoordinate(latitude, longitude, heigth);
}

GeodeticCoordinate CoordinateConvertion::convertECItoGeodetic(const ECICoordinate &eciCoord, double jd)
{
    // Konstanten:
    static const float f = 1.0 / 298.26;
    static const float eSquare = (2 - f) * f;

    const double xsquare{eciCoord.x * eciCoord.x};
    const double ysquare{eciCoord.y * eciCoord.y};

    // berechne longitude im geodetischen Referenzfenster:
    double longitude{atan2(eciCoord.y, eciCoord.x) - Calendar::computeGMST(jd)};

    // Intervallprüfung für longitude: (-PI; PI]
    if (longitude > M_PI)
        longitude -= 2 * M_PI;
    if (longitude <= -M_PI)
        longitude += 2 * M_PI;

    // lambda und phi_1 sind bekannt (= Geocentric coordinate)
    const double R{sqrt(xsquare + ysquare)};
    double phi{atan(eciCoord.z / R)}; // R > 0

    // vorheriges phi
    double phi_prev{0.0};

    // Variablen für Berechnungen:
    double C{0.0};
    double sinPhi{0.0};

    // Berechnen der Latitude auf dem Ellipsoid:
    do
    {
        phi_prev = phi;
        sinPhi = sin(phi_prev);
        C = 1.0 / sqrt(1 - eSquare * sinPhi * sinPhi);
        phi = atan((eciCoord.z + Re * C * eSquare * sinPhi) / R); // R > 0
    } while (fabs(phi_prev - phi) > 1e-6);

    const double latitude{phi};

    // Berechnen der Höhe:
    sinPhi = sin(phi);
    C = 1.0 / sqrt(1 - eSquare * sinPhi * sinPhi);

    const double heigth{R / cos(phi) - Re * C}; // cos( phi ) > 0 (Wirklich?)

    return GeodeticCoordinate(latitude, longitude, heigth);
}