

// eigener Code
#include "calendar.h"


#define plot(x) std::cout << x << std::endl;


// GLOBALE VARIABLEN
static const std::array<uint8_t, 12> days_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Array das gregorianischen Kalender repräsentiert. Aufrufender Code
    // kann Array klonen und ggf. bei Schaltjahr anpassen.



// INLINE FUNKTIONEN

// Gibt abgerundete Zahl eines Double (nur unsigned) zurück.
inline int Calendar::rnddown(double decimal) { 
    return static_cast<int>(decimal);
}

// Gibt aufgerundete Zahl eines Double (nur unsigned) zurück.
inline int Calendar::rndup(double decimal) {
    return static_cast<int>(std::trunc(decimal));
}

// Liefert Nachkommastellen (0.xx..) für T (float -od.- double) zurück.
template <typename T> inline double Calendar::getfrac(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return static_cast<T>(fractional);
}

// Gibt den Ganzzahlanteil eines Double zurück.
inline int Calendar::getint(double decimal) {
    double integral = 0;
    double fractional = modf(decimal, &integral);
    return static_cast<int>(integral);
}


// HILFSFUNKTIONEN

// Noch nicht ganz brauchbar: Erfordert als Eingabe das Jahr und keine Julian Day Fraction
 bool Calendar::checkforLeapYear_julian(uint16_t year) {
    // Jahr ist nach julianischem Kalender Schaltjahr, wenn...
    // Jahr durch 4 teilbar ist:
    return (year % 4 == 0);
}


// Prüft ob übergebenes Jahr nach gregorianischem Kalender ein Schaltjahr ist (nur A.C.)
bool Calendar::checkforLeapYear_gregorian(uint16_t year) {
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
    return ( A && ( !B | C ) ); // WICHTIG: kein || verwenden - ALLE beiden Ausdrücke der Oder-Klammer MÜSSEN 
    // ausgewertet werden um festzustellen ob das Jahr ein Schaltjahr ist, da die Regeln aufeinander aufbauen!
    // Wenn das Jahr nicht durch 4 teilbar ist, brauchen auch die beiden anderen Regeln nicht angeschaut zu werden
}

// Gibt abhängig von Schaltjahr/Nicht-Schaltjahr und dem Tag des Jahres, den Tag des Monats zurück (z.B. 206 Day of Year = 24. Tag des Monats Juli)
uint16_t Calendar::getDayOfMonth(uint16_t year, uint16_t days) {   
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;

    // Auf Schaltjahr prüfen
    bool leapyear = checkforLeapYear_gregorian(year);

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
uint8_t Calendar::getMonthOfYear(int16_t year, uint16_t days) {
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;
    
    // Auf Schaltjahr prüfen
    bool leapyear = checkforLeapYear_gregorian(year);

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
double Calendar::computeJD(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t milisecond, uint8_t microsecond) {
    // ****************************************************************************************
    // Fallunterscheidung:
    //  Ist eine Umsortierung der Monate Januar und Februar ans Ende des Jahres um Probleme
    //  durch die Verkürzung des Februars zu umgehen. Ein eventueller Schalttag ist damit stets
    //  der letzte Tag des Jahres
    if (month <= 2) { 
        year--; month += 12; 
    }
    //  Im Fall (month > 2) können alle Werte ohne Änderungen weiterverwendet werden.
    // ****************************************************************************************

    // ****************************************************************************************
    // Einzelne Summen berechnen: (Quelle siehe Funktionsende!)
    uint16_t _A = year - getint( ( 12 - month ) / 10 ); // Intervall bis 65.535
    uint8_t _M = ( month - 3 ) % 12; // Intervall: [0; 11]

    uint32_t y = getint( 365.25 * ( _A + 4712 ) ); // Intervall bis 25.657.716
    uint16_t d = getint( ( 30.6001 * _M ) + 0.5 ); // Intervall bis 428 

    uint32_t N = y + d + day + 59; // Intervall: bis 25.658.569

    uint16_t g = getint( getint( ( _A / 100 ) + 49 ) * 0.75 ) - 38; // Intervall bis 490
    // ****************************************************************************************

    // ****************************************************************************************
    // Nachkommastellen erstellen:
    //  Stunden bis Mikrosekunden
    float frac =    hour / 24.0 +
                    minute / ( 24.0 * 60.0 ) +
                    second / ( 24.0 * 60.0 * 60.0 ) +
                    milisecond / ( 24.0 * 60.0 * 60.0 * 100.0 ) +
                    microsecond / ( 24.0 * 60.0 * 60.0 * 100.0 * 100.0 )
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
    *
    *     QUELLE: Kenneth R. Lang. 1999. Astrophysical Formulae (3. Aufl)., Berlin, Deutschland. S. 72/73 
    * 
    */
    return JD;
}

// Berechnet das Datum im julianischen Kalender als Julian Day Number mit Tagesbruch (Fraction)
double Calendar::computeJD(int16_t year, double dayFraction) {
    long int A, B;
    A = 0;
    B = 0;

    year--;
    A = static_cast<long int>( year / 100 );
    B = 2 - A + static_cast<long int>( A / 4 );

    long double JD =    static_cast<long int>( 365.25 * year ) +
                        static_cast<long int>( 30.6001 * 14 ) +
                        1720994.5 +
                        B +
                        dayFraction
                        ;

    /*
    *
    *  QUELLE: https://www.celestrak.com/columns/v02n02/
    * 
    */


    return JD;
}




double Calendar::computeGMST(double jd) {
    // (1) Using the Julian date, JD, and the universal time, UT, in hours, calculate T, the number of centuries from J2000
    //double T = (jd + getUTTime() / 24 - 2451545.0) / 36525;
    // (2) Calculate the mean longitude corrected for aberration, L; the mean
    // anomaly, G; the ecliptic longitude, lambda; and the obliquity of the ecliptic, e:

    /*
    QUELLE: https://books.google.de/books?id=Nq_1CAAAQBAJ&pg=PA73&lpg=PA73&dq=Julian+Date+Number+in+Greenwich+Meridian+Startime&source=bl&ots=JIHPd5iyiA&sig=ACfU3U3MvCsSi1-C74qO7rnnpcQrxyLnsA&hl=de&sa=X&ved=2ahUKEwjV8KuS_bvpAhUM-qQKHWVkBAMQ6AEwAHoECAoQAQ#v=onepage&q=Julian%20Date%20Number%20in%20Greenwich%20Meridian%20Startime&f=false
    */


    float hour_frac = getfrac<float>( jd) ;

    float T = ( jd + ( hour_frac / 24 ) - 2451545.0 ) / 36525;

    float L = 280.460 + 36000.770 * T;

    float G = 357.528 + 35999.050 * T;

    float lambda = L + 1.915 * sinf( G ) + 0.020 * sinf( 2 * G );

    float e = 23.4393 - 0.01300 * T;

    float E = -1.915 * sinf( G ) - 0.020 * sinf( 2 * G ) + 2.466 * sinf( 2 * lambda ) - 0.053 * sinf( 4 * lambda );

    float GHA = 15 * hour_frac - 180 * E;

    float delta = asinf( sinf( e ) * sinf( lambda ) );

    float SD = 0.267 / ( 1 - 0.017 * cosf( G ) );

    return SD;
}