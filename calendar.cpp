// Standartbibliothek
#include <cmath>
#include <array>
#include <cstdint>
#include <iostream>


#define plot(x) std::cout << x << std::endl;


// GLOBALE VARIABLEN

const std::array<uint8_t, 12> days_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Array mit 12 Felder die den Monaten entsprechen und mit
// Anzahl Tagen gefüllt sind. Wird für Monats- und Tagesberechnung benötigt.


// INLINE FUNKTIONEN

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


// HILFSFUNKTIONEN

bool checkforLeapYear(uint16_t year) {
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
    return ( A && ( !B | C ) ); // WICHTIG: kein && -oder- || verwenden - ALLE drei Ausdrücke MÜSSEN 
    // ausgewertet werden um festzustellen ob das Jahr ein Schaltjahr ist, da die Regeln aufeinander aufbauen!
}

// Gibt abhängig von Schaltjahr/Nicht-Schaltjahr und dem Tag des Jahres, den Tag des Monats zurück (z.B. 206 Day of Year = 24. Tag des Monats Juli)
int getDayOfMonth(uint16_t year, uint16_t days) {   
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;

    // Auf Schaltjahr prüfen
    bool leapyear = checkforLeapYear(year);

    if( leapyear ) {
        if (days > 366) return 0; // Bei zu vielen Tagen 0 zurückgeben, da sonst in Schleife Overflow verursacht würde

        sub_days_month[1]++; // Schalttag bei Februar hinzufügen
    }
    else
    {
        if (days > 365) return 0; // siehe oben
    }

    // Nachdem Jahrestyp feststeht, beginnen, den Tag des Monats herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis 
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    for (uint8_t i = 0; days > sub_days_month[i]; i++)
    {
        days -= sub_days_month[i];
    }
    
    // Schleife bricht ab, wenn aktuelle Anzahl Tage kleiner ist als Anzahl Gesamttage des aktuellen Monats
    // dann sind wir im richtigen Monat gelandet
    return days;
}

// Liefert für die Anzahl der Tage des Jahres in Abhängigkeit von Schaltjahr/Nicht-Schaltjahr, den Monat in dem sich der Tag befindet (z.B. für 206 Day of Year = Monat Juli)
int getMonthOfYear(uint16_t year, uint16_t days) {
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;
    
    // Auf Schaltjahr prüfen
    bool leapyear = checkforLeapYear(year);

    if( leapyear ) {
        if (days > 366) return 0; // Bei zu vielen Tagen 0 zurückgeben, da sonst in Schleife Overflow verursacht würde

        sub_days_month[1]++; // Schalttag bei Februar hinzufügen
    }
    else
    {
        if (days > 365) return 0; // siehe oben
    }
    
    // Nachdem Jahrestyp feststeht, beginnen, den Monat des Jahres herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis 
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    uint8_t counter = 1; // Monatszähler. Startet im Januar (0 als Start ergäbe keinen Sinn, wäre außerhalb des Monats)

    for (size_t i = 0; days > sub_days_month[i]; i++)
    {
        days -= sub_days_month[i];
        counter++;
    }
    
    // Nach Prinzip getDayOfYear: Wenn Tage kleiner sind als ein voller Monat, sind wir im richtigen Monat gelandet. 
    // Dann einfach Zähler zurückgeben:
    return counter;
}

// Berechnet die Julian Day Number entsprechend des Jahres und des Tagebruchs.
double computeJD(int year, double dayFractions) {
    // ****************************************************************************************
    // Ab 'Stunde' die Gleitkommazahlen speichern für Umrechnungen:
    double hour_frac = exfrac<double>(dayFractions) * 24; 
    double minute_frac = exfrac<double>(hour_frac) * 60;
    float second_frac = exfrac<float>(minute_frac) * 60; // ab hier (second_frac bis microsecond_frac) wirkt sich größere Genauigkeit
    // durch mehr Nachkommastellen in 'double' nur noch marginal aus. Es wird stattdessen 'float' verwendet um Speicher zu sparen.
    float milisecond_frac = exfrac<float>(second_frac) * 100; // 100 ms = 1 s
    float microsecond_frac = exfrac<float>(milisecond_frac) * 100; // 100 mikrosec = 1 ms 
    // ****************************************************************************************

    // ****************************************************************************************
    // Alle Variablen in Ganzzahltypen umwandeln:
    //  Wichtig: Hier noch nicht mit Julianischen Konstanten rechnen, da es sich noch um "gregorianische" Größen handelt.
    //  Die Umrechnung beginnt erst später in der Berechnung des JDs.
    uint8_t months = getMonthOfYear(year, exint(dayFractions)); //uabrnd(dayFractions / 30.6001); // Intervall: [0; 11] (Ausnahme von obiger Regel: Liefert hinreichend genaue Angaben des Monats)
    uint16_t days = getDayOfMonth(year, exint(dayFractions)); // Intervall: [0; 365/366]
    uint8_t hours = exint(hour_frac); // Intervall: [0; 24)
    uint8_t minutes = exint(minute_frac); // Intervall: [0; 60)
    uint8_t seconds = exint(second_frac); // Intervall: [0; 60)
    uint8_t miliseconds = exint(milisecond_frac); // Intervall: [0; 100)
    uint8_t microseconds = exint(microsecond_frac); // Intervall: [0; 100)
    // ****************************************************************************************

    // ****************************************************************************************
    // Fallunterscheidung:
    //  Ist eine Umsortierung der Monate Januar und Februar ans Ende des Jahres um Probleme
    //  durch die Verkürzung des Februars zu umgehen. Ein eventueller Schalttag ist damit stets
    //  der letzte Tag des Jahres
    if (months <= 2) { 
        year--; months += 12; 
    }
    //  Im Fall (month > 2) können alle Werte ohne Änderungen weiterverwendet werden.
    // ****************************************************************************************

    // ****************************************************************************************
    // Einzelne Summen berechnen: (Quelle siehe Funktionsende!)
    uint16_t _A = year - exint( ( 12 - months ) / 10 ); // Intervall bis 65.535
    uint8_t _M = ( months - 3 ) % 12; // Intervall: [0; 11]

    uint32_t y = exint( 365.25 * ( _A + 4712 ) ); // Intervall bis 25.657.716
    uint16_t d = exint( ( 30.6001 * _M ) + 0.5 ); // Intervall bis 428 

    uint32_t N = y + d + days + 59; // Intervall: bis 25.658.569

    uint16_t g = exint( exint( ( _A / 100 ) + 49 ) * 0.75 ) - 38; // Intervall bis 490
    // ****************************************************************************************

    // ****************************************************************************************
    // Nachkommastellen erstellen:
    //  Stunden bis Mikrosekunden
    float frac =    hours / 24.0 +
                    minutes / ( 24.0 * 60.0 ) +
                    seconds / ( 24.0 * 60.0 * 60.0 ) +
                    miliseconds / ( 24.0 * 60.0 * 60.0 * 100.0 ) +
                    microseconds / ( 24.0 * 60.0 * 60.0 * 100.0 * 100.0 )
                    ;
    // ****************************************************************************************

    // ****************************************************************************************
    // JD aus allen Variablen  zusammensetzen:
    long double JD = 1.0*N - 1.0*g + frac - 0.5; // - 0.5 folgt aus der unterschiedlichen Tagesdefinition der beiden Kalender:
    // Gregorianischer Kalender geht von 0-24 Uhr
    // Julianischer Kalender geht von 12-12
    // ****************************************************************************************

    // Formel aus der Quelle bezieht Zeit nur bis Stunden ein, Minuten bis Mikrosekunden wurden
    // von mir hinzugefügt. Darauf achten: Ab Sekunde ist Umrechnungsfaktor 100, nicht 60!

    /*
        QUELLE: Kenneth R. Lang. 1999. Astrophysical Formulae (3. Aufl)., Berlin, Deutschland. S. 72/73 
    */
    return JD;
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





   return 0;
}

//GeocentricCoordinate convertECItoGeocentric(const ECICoordinate& eciCoord, double jd);

//GeodeticCoordinate convertECItoGeodetic(const ECICoordinate& eciCoord, double jd);