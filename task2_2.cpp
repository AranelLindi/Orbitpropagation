// Standartbibliothek
#include <cmath> // für trunc
#include <cstdint> // für int-Typen

// eigener Code
#include "task2_2.h"



double computeJD(int year, double dayFractions) {
    u_int16_t a, m, d, h, s;
    double h;

    a = year; // Jahr
    d = std::trunc( dayFractions ); // Tage
    m = std::trunc( d / 30.6001 );

    double test; // was enthält test? AUSGABE!
    h = modf( dayFractions, &test );

    // Fallunterscheidung:
    if ( m <= 2 ) {
        a--;
        m += 12;
    }
    // Für den Fall das m > 2, nichts unternehmen (Werte bleiben so)

    double JD = (int)( 365.25 * a ) +
                (int)( 30.6001 * (m + 1) ) +
                d +
                ( h / 24.0 ) +
                1720981.5
                ;
    return JD;
}    

double computeGMST(double jd);

GeocentricCoordinate convertECItoGeocentric(const ECICoordinate& eciCoord, double jd);

GeodeticCoordinate convertECItoGeodetic(const ECICoordinate& eciCoord, double jd);