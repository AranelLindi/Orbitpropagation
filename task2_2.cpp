// Standartbibliothek
#include <cmath>
#include <cstdint>
#include <iostream>



#define plot(x) std::cout << x << std::endl;

// Gibt abgerundete Zahl eines Double (nur größer 0) zurück.
inline int uabrnd(double decimal) { 
    return static_cast<int>(decimal);
}

// Gibt aufgerundete Zahl eines Double zurück.
inline int uaufrnd(double decimal) {
    return static_cast<int>(std::ceil(decimal));
}

// Liefert Nachkommastellen (0.xx..) eines Double zurück.
inline double exfrac(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return fractional;
}

// Extract Number, gibt den Ganzzahlanteil eines Double zurück.
inline int exint(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return static_cast<int>(integral);
}

// Berechnet die Julian Day Number entsprechend des Jahres und des Tagesbruchs.
double computeJD(int year, double dayFractions) {
    // Ab Stunde die Gleitkommazahlen speichern für Umrechnungen:
    double hourfrac = exfrac(dayFractions) * 24;
    double minutefrac = exfrac(hourfrac) * 60;
    double secondfrac = exfrac(minutefrac) * 60;
    double milisecondfrac = exfrac(secondfrac) * 60;
    double mikrosecondfrac = exfrac(milisecondfrac) * 60;

    // Alle Variablen in Ganzzahltypen umwandeln:
    uint16_t years = year;
    uint16_t months = uabrnd(dayFractions / 30.6001);
    uint16_t days = uabrnd(dayFractions) - uabrnd(months * 30.6001) + 1;
    uint16_t hours = exint(hourfrac);
    uint16_t minutes = exint(minutefrac);
    uint16_t seconds = exint(secondfrac);
    uint16_t miliseconds = exint(milisecondfrac);
    uint16_t mikroseconds = exint(mikrosecondfrac);

    // ****************************************************************************************
    // Noch prüfen ob Fallunterscheidung wirklich wegfällt!! (Beispiele ausprobieren!)

    // Fallunterscheidung: month ist größer 0 wird vorausgesetzt (Datentyp unsigned & dayFraction > 0)
    //if (months <= 2) { plot("it happened!");years--; months += 12; }
    //else { ++months; }
    // ****************************************************************************************

    ++months; // Monate um 1 inkrementieren, da Julian Calender 0-basierte Monatsindexierung führt
    // vermutlich fällt deswegen auch hier die Fallunterscheidung weg, da die unter dem Link
    // angegebene Formel dies nicht erwähnt und ab 1 bis 12 zu zählen beginnt.

    double JD = exint( 365.25 * years ) +
                exint( 30.6001 * (months + 1) ) +
                days +
                ( hours / 24.0 ) +
                ( minutes / ( 24.0 * 60.0 ) ) +
                ( seconds / ( 24.0 * 60.0 * 60.0 ) ) +
                ( miliseconds / ( 24.0 * 60.0 * 60.0 * 100.0 ) ) +
                ( mikroseconds / ( 24.0 * 60.0 * 60.0 * 100.0 * 100.0 ) ) +
                1720981.5
                ;
    
    return JD;

    /*
    QUELLE: https://gssc.esa.int/navipedia/index.php/Julian_Date
    */
}    

int getUTTime(void) { return 0; } // Noch zu implementieren. Sie unten!

double computeGMST(double jd) {
    // (1) Using the Julian date, JD, and the universal time, UT, in hours, calculate T, the number of centuries from J2000
    double T = (jd + getUTTime() / 24 - 2451545.0) / 36525;
    // (2) Calculate the mean longitude corrected for aberration, L; the mean
    // anomaly, G; the ecliptic longitude, lambda; and the obliquity of the ecliptic, e:

    /*
    QUELLE: https://books.google.de/books?id=Nq_1CAAAQBAJ&pg=PA73&lpg=PA73&dq=Julian+Date+Number+in+Greenwich+Meridian+Startime&source=bl&ots=JIHPd5iyiA&sig=ACfU3U3MvCsSi1-C74qO7rnnpcQrxyLnsA&hl=de&sa=X&ved=2ahUKEwjV8KuS_bvpAhUM-qQKHWVkBAMQ6AEwAHoECAoQAQ#v=onepage&q=Julian%20Date%20Number%20in%20Greenwich%20Meridian%20Startime&f=false
    */
}

//GeocentricCoordinate convertECItoGeocentric(const ECICoordinate& eciCoord, double jd);

//GeodeticCoordinate convertECItoGeodetic(const ECICoordinate& eciCoord, double jd);