// Standartbibliothek
#include <cmath>
#include <cstdint>
#include <iostream>


#define plot(x) std::cout << x << std::endl;


// Gibt abgerundete Zahl eines Double (nur unsigned) zurück.
inline int uabrnd(double decimal) { 
    return static_cast<int>(decimal);
}

// Gibt aufgerundete Zahl eines Double (nur unsigned) zurück.
inline int uaufrnd(double decimal) {
    return static_cast<int>(std::ceil(decimal));
}

// Liefert Nachkommastellen (0.xx..) für T (float -od.- double) zurück.
template <typename T> inline double exfrac(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return static_cast<T>(fractional);
}

// Extract Number, gibt den Ganzzahlanteil eines Double zurück.
inline int exint(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return static_cast<int>(integral);
}

// Gibt abhängig von Schaltjahr/Nicht-Schaltjahr und dem Tag des Jahres, den Tag des Monats zurück (z.B. 206 Day of Year = 24. Tag des Monats Juli)
int getDayOfMonth(int year, uint16_t days) {
    uint8_t days_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Prüfen ob year ein Schaltjahr ist, falls ja, dann zu Februar (days_month[1] einen Tag addieren!)
    
    // Schaltjahr-Regel für gregorianischen Kalender:
    // 1. Regel: Jahr ist durch 4 teilbar -> Schaltjahr
    // 2. Regel: Jahr ist durch 100 teilbar -> KEIN Schaltjahr
    // 3. Regel: Jahr ist durch 400 teilbar -> Doch wieder Schaltjahr...

    // Übersetzen in vier Wahrheitsaussagen:
    // A = Jahr ist durch 4 teilbar
    // B = Jahr ist durch 100 teilbar
    // C = Jahr ist durch 400 teilbar

    bool A, B, C;
    A = (year % 4 == 0);
    B = (year % 100 == 0);
    C = (year % 400 == 0);

    // Aussagenbeschreibung: Jahr ist ein Schaltjahr genau dann wenn...
    // Jahr durch 4 teilbar ist UND ( es NICHT durch 100 teilbar ist ODER es durch 400 teilbar ist)    
    if ( A & ( !B | C ) ) days_month[1]++; // WICHTIG: kein && oder || verwenden - ALLE drei Ausdrücke MÜSSEN ausgewertet werden!

    // Nachdem Jahrestyp feststeht, beginnen, den Tag des Monats herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis 
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    for (size_t i = 0; days > days_month[i]; i++)
    {
        days -= days_month[i];
    }
    // Wird eine Tageszahl größer als 365/366 übergeben, verursacht diese Funktion einen Array Overflow!
    
    // Schleife bricht ab, wenn Tage kleiner sind als Anzahl Tage des aktuellen Monats, dann sind wir im richtigen Monat gelandet
    return days;
}

// Berechnet die Julian Day Number entsprechend des Jahres und des Tagebruchs.
double computeJD(int year, double dayFractions) {
    // Ab Stunde die Gleitkommazahlen speichern für Umrechnungen:
    double hourfrac = exfrac<double>(dayFractions) * 24; 
    double minutefrac = exfrac<double>(hourfrac) * 60;
    float secondfrac = exfrac<float>(minutefrac) * 60; // ab hier wirkt sich größere Genauigkeit bei Double nur noch 
    float milisecondfrac = exfrac<float>(secondfrac) * 100; // 100 ms = 1 s
    float mikrosecondfrac = exfrac<float>(milisecondfrac) * 100; // 100 mikros = 1 ms 

    // Alle Variablen in Ganzzahltypen umwandeln:
    // Wichtig: Hier noch nicht mit Julianischen Konstanten rechnen, da es sich noch um "gregorianische" Größen handelt.
    // Die Umrechnung beginnt erst später in der Berechnung des JDs.
    uint16_t years = year; // Intervall: [0; 2099] negative Jahreszahlen werden nicht berücksichtigt!
    uint8_t months = uabrnd(dayFractions / 30.6001); // Intervall: [0; 11] (Ausnahme von obiger Regel: Liefert hinreichend genaue Angaben des Monats)
    uint16_t days = getDayOfMonth(year, exint(dayFractions));//uabrnd(dayFractions - (months * 30.6001)) + 1; // Intervall: [0; 365]
    uint8_t hours = exint(hourfrac); // Intervall: [0; 24)
    uint8_t minutes = exint(minutefrac); // Intervall: [0; 60)
    uint8_t seconds = exint(secondfrac); // Intervall: [0; 60)
    uint8_t miliseconds = exint(milisecondfrac); // Intervall: [0; 100)
    uint8_t mikroseconds = exint(mikrosecondfrac); // Intervall: [0; 100)

    /*plot(hourfrac)
    plot(minutefrac)
    plot(secondfrac)
    plot(milisecondfrac)
    plot(mikrosecondfrac)
    plot("----")
    plot(years)
    plot((int)months)*/
    plot(days)
    /*plot((int)hours)
    plot((int)minutes)
    plot((int)seconds)
    plot((int)miliseconds)
    plot((int)mikroseconds)*/

    // ****************************************************************************************
    // Noch prüfen ob Fallunterscheidung wirklich wegfällt!! (Beispiele ausprobieren!)

    // Fallunterscheidung: month ist größer 0 wird vorausgesetzt (Datentyp unsigned & dayFraction > 0)
    //if (months <= 2) { years--; months += 12; }
    //else { ++months; }
    // ****************************************************************************************
    
    //++days; // evtl wieder löschen. Ein Tag fehlt nur manchmal... Vielleicht auch erklärbar damit, dass vom 0-ten Tag an gezählt wird?

    ++months; // Monate um 1 inkrementieren, da Julian Calender 0-basierte Monatsindexierung führt
    // vermutlich fällt deswegen auch hier die Fallunterscheidung weg, da die unter dem Link
    // angegebene Formel dies nicht erwähnt und ab 1 bis 12 zu zählen beginnt.


    long double JD =    exint( 365.25 * years ) +
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
    
    // Formel aus der Quelle bezieht Zeit nur bis Stunden ein, Minuten bis Mikrosekunden wurden
    // von mir hinzugefügt. Darauf achten: Ab Sekunde ist Umrechnungsfaktor 100, nicht 60!

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