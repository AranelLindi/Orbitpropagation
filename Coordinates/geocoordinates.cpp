#include "geocoordinates.h"

// Print-Funktion der Koordinaten-Basisklasse:
// (wird von beiden Objekten jeweils in der eigenen print-Funktion aufgerufen)
void BaseCoordinate::print(void)
{
    std::cout << '\t' << "latitude:" << '\t' << this->latitude << '\n'
              << "\t\t\t"
              << "longitude:" << '\t' << this->longitude << '\n'
              << "\t\t\t"
              << "height:" << '\t' << this->heigth << std::endl;
}

// Ausgabe Geodetische Koordinate:
void GeodeticCoordinate::print(void)
{
    std::cout << "GeodeticCoordinate:";
    BaseCoordinate::print();
}

// Ausgabe Geozentrische Koordinate:
void GeocentricCoordinate::print(void)
{
    std::cout << "GeocentricCoordinate:";
    BaseCoordinate::print();
}


GeocentricCoordinate CoordinateConvertion::convertECItoGeocentric(const ECICoordinate& eciCoord, double jd) {
    GeocentricCoordinate geocCoord;

    double xsquare = eciCoord.x * eciCoord.x;
    double ysquare = eciCoord.y * eciCoord.y;
    double zsquare = eciCoord.z * eciCoord.z;

    geocCoord.latitude = atan( eciCoord.z / sqrt(xsquare + ysquare) ); // Nenner > 0
    geocCoord.longitude = atan2( eciCoord.y, eciCoord.x ) - Calendar::computeGMST(jd);

    // Intervallprüfung longitude zwischen (-PI; PI]
    if (geocCoord.longitude > M_PI)
        geocCoord.longitude -= 2 * M_PI;
    if (geocCoord.longitude <= -M_PI)
        geocCoord.longitude += 2 * M_PI;

    geocCoord.heigth = sqrt( xsquare + ysquare + zsquare ) - Re;

    return geocCoord;
}

GeodeticCoordinate CoordinateConvertion::convertECItoGeodetic(const ECICoordinate& eciCoord, double jd) {
    // Konstanten:
    static const float f = 1.0 / 298.26; 
    static const float eSquare = (2 - f) * f;
    
    GeodeticCoordinate geodCoord;

    double xsquare = eciCoord.x * eciCoord.x;
    double ysquare = eciCoord.y * eciCoord.y;

    // berechne longitude im geodetischen Referenzfenster:
    geodCoord.longitude = atan2( eciCoord.y, eciCoord.x ) - Calendar::computeGMST(jd);

    // Intervallprüfung für longitude: (-PI; PI]
    if (geodCoord.longitude > M_PI)
        geodCoord.longitude -= 2 * M_PI;
    if (geodCoord.longitude <= -M_PI)
        geodCoord.longitude += 2 * M_PI;
    
    // lambda und phi_1 sind bekannt (= Geocentric coordinate)
    double R = sqrt( xsquare + ysquare );
    double phi = atan( eciCoord.z / R ); // R > 0

    // vorheriges phi
    double phi_prev = 0;

    // variablen für Berechnungen:
    double C = 0;
    double sinPhi = 0;

    // berechnen der latitude auf dem Ellipsoid:
    do
    {
        phi_prev = phi;
        sinPhi = sin( phi_prev );
        C = 1.0 / sqrt( 1 - eSquare * sinPhi * sinPhi );
        phi = atan( ( eciCoord.z + Re * C * eSquare * sinPhi ) / R ); // R > 0
    } while ( fabs(phi_prev - phi) > 1e-6 );
    
    geodCoord.latitude = phi;

    // berechnen der Höhe:
    sinPhi = sin( phi );
    C = 1.0 / sqrt( 1 - eSquare * sinPhi * sinPhi );
    geodCoord.heigth = R / cos( phi ) - Re * C; // cos( phi ) > 0 (Wirklich?)

    return geodCoord;
}