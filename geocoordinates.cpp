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
void GeodeticCoordinate::print(void) {
    std::cout << "GeodeticCoordinate:";
    BaseCoordinate::print();
}

// Ausgabe Geozentrische Koordinate:
void GeocentricCoordinate::print(void) {
    std::cout << "GeocentricCoordinate:";
    BaseCoordinate::print();
}