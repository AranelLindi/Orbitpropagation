// Standartbibliothek
#include <cmath>
#include <array>
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
    return ( A & ( !B | C ) ); // WICHTIG: kein && oder || verwenden - ALLE drei Ausdrücke MÜSSEN 
    // ausgewertet werden!
}

// Hier const definieren, von aufrufenden Funktionen klonen lassen. Vorteil: Nur hier dauerhaft änderbar.
const std::array<uint8_t, 12> days_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// Gibt abhängig von Schaltjahr/Nicht-Schaltjahr und dem Tag des Jahres, den Tag des Monats zurück (z.B. 206 Day of Year = 24. Tag des Monats Juli)
int getDayOfMonth(uint16_t year, uint16_t days) {
    //uint8_t days_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; Löschen wenn das Klonen auch funktioniert!
    
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;
    uint8_t& february = sub_days_month[1]; // Zeigt auf zweites Arrayfeld (entspr. Februar)
    
    // Auf Schaltjahr prüfen
    if ( checkforLeapYear(year) ) february++; // und falls ja, Tage des Februar um 1 erhöhen (29. Februar)

    // Nachdem Jahrestyp feststeht, beginnen, den Tag des Monats herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis 
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    for (uint8_t i = 0; days > sub_days_month[i]; i++)
    {
        days -= sub_days_month[i];
    }
    // Wird eine Tageszahl größer als 365/366 übergeben, verursacht diese Funktion einen Array Overflow!
    
    // Schleife bricht ab, wenn aktuelle Anzahl Tage kleiner ist als Anzahl Gesamttage des aktuellen Monats
    // dann sind wir im richtigen Monat gelandet
    return days;
}

// Liefert für die Anzahl der Tage des Jahres in Abhängigkeit von Schaltjahr/Nicht-Schaltjahr, den Monat in dem sich der Tag befindet (z.B. für 206 Day of Year = Monat Juli)
int getMonthOfYear(uint16_t year, uint16_t days) {
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;
    uint8_t& february = sub_days_month[1]; // Zeigt auf zweites Arrayfeld (entspr. Februar)

    // Auf Schaltjahr prüfen
    if ( checkforLeapYear(year) ) february++; // und falls ja, Tage des Februar um 1 erhöhen (29. Februar)

    // Nachdem Jahrestyp feststeht, beginnen, den Monat des Jahres herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis 
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    uint8_t counter = 0; // Monatszähler

    while (days > sub_days_month[counter])
    {
        days -= sub_days_month[counter];
        counter++; // Monat inkrementieren
    } // zum nächsten Monat übergehen
    
    // Nach Prinzip getDayOfYear: Wenn Tage kleiner sind als ein voller Monat, sind wir im richtigen Monat gelandet. 
    // Dann einfach Zähler zurückgeben:
    return counter;
}

// Berechnet die Julian Day Number entsprechend des Jahres und des Tagebruchs.
double computeJD(int year, double dayFractions) {
    // Ab 'Stunde' die Gleitkommazahlen speichern für Umrechnungen:
    double hour_frac = exfrac<double>(dayFractions) * 24; 
    double minute_frac = exfrac<double>(hour_frac) * 60;
    float second_frac = exfrac<float>(minute_frac) * 60; // ab hier (Secondfrac bis Microsecondfrac) wirkt sich 
    // größere Genauigkeit bei double nur noch marginal aus. Es wird float verwendet um Speicher zu sparen.
    float milisecond_frac = exfrac<float>(second_frac) * 100; // 100 ms = 1 s
    float microsecond_frac = exfrac<float>(milisecond_frac) * 100; // 100 mikrosec = 1 ms 

    // Alle Variablen in Ganzzahltypen umwandeln:
    // Wichtig: Hier noch nicht mit Julianischen Konstanten rechnen, da es sich noch um "gregorianische" Größen handelt.
    // Die Umrechnung beginnt erst später in der Berechnung des JDs.
    uint16_t years = year; // Intervall: [0; 2099] negative Jahreszahlen werden nicht berücksichtigt!
    uint8_t months = getMonthOfYear(year, exint(dayFractions)); //uabrnd(dayFractions / 30.6001); // Intervall: [0; 11] (Ausnahme von obiger Regel: Liefert hinreichend genaue Angaben des Monats)
    uint16_t days = getDayOfMonth(year, exint(dayFractions)); // Intervall: [0; 365/366]
    uint8_t hours = exint(hour_frac); // Intervall: [0; 24)
    uint8_t minutes = exint(minute_frac); // Intervall: [0; 60)
    uint8_t seconds = exint(second_frac); // Intervall: [0; 60)
    uint8_t miliseconds = exint(milisecond_frac); // Intervall: [0; 100)
    uint8_t mikroseconds = exint(microsecond_frac); // Intervall: [0; 100)

    // ****************************************************************************************
    // Fallunterscheidung: *(month ist größer 0 wird vorausgesetzt (Datentyp unsigned & dayFraction > 0))*
    // Dient dazu, um Fehler, verursacht durch den Februar (weniger Tage als der Durchschnitt) und durch Schalttage
    // abzufangen und zu umgehen. Beispiel:
    // Ohne Fallunterscheidung führt dayFraction 60 (in Schaltjahr 2020 ist das 29. Februar) zu JD von 2458906.5
    // mit Fallunterscheidung                                                                zu JD von 2458908.5
    // also zwei Tage Unterschied!
    if (months <= 2) { years--; months += 12; }
    // Im Fall (month > 2) können alle Werte ohne Änderungen weiterverwendet werden.
    // ****************************************************************************************
    
    plot((int)months)
    plot(days)

    // ****************************************************************************************
    ++months; // Monate um 1 inkrementieren, da Julian Calender 0-basierte Monatsindexierung führt
    // Quelle: [NOCH EINFÜGEN]
    // ****************************************************************************************
    // Berechnung nach der unter QUELLE angegebenen Formel durchführen. Minuten bis Mikrosekunden
    // wurden händisch hinzugefügt um Genauigkeit zu erhöhen. Nach dem in der Formel angegebenen
    // Prinzip für Stunden, werden alle weitere Einheiten in Stunden umgerechnet und aufaddiert.
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
    // ****************************************************************************************

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