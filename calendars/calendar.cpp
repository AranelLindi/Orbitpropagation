// eigener Code
#include "calendar.h" // HEADER

//#define plot(x) std::cout << x << std::endl; // nur für Debugging-Zwecke

// GLOBALE VARIABLEN
static const constexpr std::array<uint8_t, 12> days_month = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; // Array das gregorianischen Kalender repräsentiert. Aufrufender Code
    // kann Array klonen und ggf. bei Schaltjahr anpassen.

// HILFSFUNKTIONEN

// Noch nicht ganz brauchbar: Erfordert als Eingabe das Jahr und keine Julian Day Fraction
/*bool Calendar::checkforLeapYear_julian(uint16_t year)
{
    // Jahr ist nach julianischem Kalender Schaltjahr, wenn...
    // Jahr durch 4 teilbar ist:
    return (year % 4 == 0);
}*/

// Prüft ob übergebenes Jahr nach gregorianischem Kalender ein Schaltjahr ist (nur A.C.)
bool Calendar::checkforLeapYear_gregorian(uint16_t year)
{
    // Schaltjahr-Regel für gregorianischen Kalender:
    // 1. Regel: Jahr ist durch 4 teilbar -> Schaltjahr
    // 2. Regel: Jahr ist durch 100 teilbar -> KEIN Schaltjahr
    // 3. Regel: Jahr ist durch 400 teilbar -> Doch wieder Schaltjahr...

    // Übersetzen in vier Wahrheitsaussagen:
    // A = Jahr ist durch 4 teilbar
    // B = Jahr ist durch 100 teilbar
    // C = Jahr ist durch 400 teilbar

    const bool A{(year % 4 == 0)}, B{(year % 100 == 0)}, C{(year % 400 == 0)};

    // Aussagenbeschreibung: Jahr ist ein Schaltjahr genau dann wenn...
    // Jahr durch 4 teilbar ist UND ( es NICHT durch 100 teilbar ist ODER es durch 400 teilbar ist)
    return (A && (!B | C)); // WICHTIG: kein || verwenden - ALLE beiden Ausdrücke der Oder-Klammer MÜSSEN
    // ausgewertet werden um festzustellen ob das Jahr ein Schaltjahr ist, da die Regeln aufeinander aufbauen!
    // Wenn das Jahr nicht durch 4 teilbar ist, brauchen auch die beiden anderen Regeln nicht angeschaut zu werden
}

// Gibt abhängig von Schaltjahr/Nicht-Schaltjahr und dem Tag des Jahres, den Tag des Monats zurück (z.B. 206 Day of Year = 24. Tag des Monats Juli)
uint16_t Calendar::getDayOfMonth(uint16_t year, uint16_t days)
{
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;

    // Auf Schaltjahr prüfen
    const bool leapyear{checkforLeapYear_gregorian(year)};

    if (leapyear)
    {
        if (days > 366)
            return 0; // Bei zu vielen Tagen 0 zurückgeben, da sonst in Schleife Overflow verursacht würde

        sub_days_month[1]++; // Schalttag bei Februar hinzufügen
    }
    else
    {
        if (days > 365)
            return 0; // siehe oben
    }

    // Nachdem Jahrestyp feststeht, beginnen, den Tag des Monats herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    for (uint8_t i = 0; days > sub_days_month[i]; i++)
        days -= sub_days_month[i];

    // Schleife bricht ab, wenn aktuelle Anzahl Tage kleiner ist als Anzahl Gesamttage des aktuellen Monats
    // dann sind wir im richtigen Monat gelandet
    return days;
}

// Liefert für die Anzahl der Tage des Jahres in Abhängigkeit von Schaltjahr/Nicht-Schaltjahr, den Monat in dem sich der Tag befindet (z.B. für 206 Day of Year = Monat Juli)
uint8_t Calendar::getMonthOfYear(int16_t year, uint16_t days)
{
    // Array klonen, ist dann auch nicht mehr const
    std::array<uint8_t, 12> sub_days_month = days_month;

    // Auf Schaltjahr prüfen
    const bool leapyear{checkforLeapYear_gregorian(year)};

    if (leapyear)
    {
        if (days > 366)
            return 0; // Bei zu vielen Tagen 0 zurückgeben, da sonst in Schleife Overflow verursacht würde

        sub_days_month[1]++; // Schalttag bei Februar hinzufügen
    }
    else
    {
        if (days > 365)
            return 0; // siehe oben
    }

    // Nachdem Jahrestyp feststeht, beginnen, den Monat des Jahres herauszufinden. Dazu so lange
    // durch Monate (Array) iterieren und die ganzen Monate von der Tageszahl abziehen, bis
    // der nächste Monat bzw. dessen Tage größer ist als die aktuelle Summe => Monat gefunden!
    uint8_t counter{1}; // Monatszähler. Startet im Januar (0 als Start ergäbe keinen Sinn, wäre außerhalb des Monats)

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
double Calendar::computeJD(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t milisecond, uint8_t microsecond)
{
    // ****************************************************************************************
    // Fallunterscheidung:
    //  Ist eine Umsortierung der Monate Januar und Februar ans Ende des Jahres um Probleme
    //  durch die Verkürzung des Februars zu umgehen. Ein eventueller Schalttag ist damit stets
    //  der letzte Tag des Jahres
    if (month <= 2)
    {
        year--;
        month += 12; // month zu Beginn bei max. 12 -> 24 < 128 !
    }
    //  Im Fall (month > 2) können alle Werte ohne Änderungen weiterverwendet werden.
    // ****************************************************************************************

    // ****************************************************************************************
    // Einzelne Summen berechnen: (Quelle siehe Funktionsende!)
    const uint16_t _A{year - getint((12 - month) / 10)}; // Intervall bis 65.535
    const uint8_t _M{(month - 3) % 12};                  // Intervall: [0; 11]

    const uint32_t y{getint(365.25 * (_A + 4712))}; // Intervall bis 25.657.716
    const uint16_t d{getint((30.6001 * _M) + 0.5)}; // Intervall bis 428

    const uint32_t N{y + d + day + 59}; // Intervall: bis 25.658.569

    const uint16_t g{getint(getint((_A / 100) + 49) * 0.75) - 38}; // Intervall bis 490
    // ****************************************************************************************

    // ****************************************************************************************
    // Nachkommastellen erstellen:
    //  Stunden bis Mikrosekunden
    const float frac = hour / 24.0f +
                       minute / (24.0f * 60.0f) +
                       second / (24.0f * 60.0f * 60.0f) +
                       milisecond / (24.0f * 60.0f * 60.0f * 100.0f) +
                       microsecond / (24.0f * 60.0f * 60.0f * 100.0f * 100.0f);
    // ****************************************************************************************

    // ****************************************************************************************
    // JD aus allen Variablen  zusammensetzen:
    const long double JD{1.0 * N - 1.0 * g + frac - 0.5}; // - 0.5 folgt aus der unterschiedlichen Tagesdefinition der beiden Kalender:
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
double Calendar::computeJD(int16_t year, double dayFraction)
{
    const int64_t A{static_cast<long int>(year / 100)}, B{2 - A + static_cast<long int>(A / 4)};

    const long double JD = static_cast<long int>(365.25f * year) +
                           static_cast<long int>(30.6001f * 14) +
                           1720994.5f +
                           B +
                           dayFraction;

    /*
    *
    *  QUELLE: https://www.celestrak.com/columns/v02n02/
    * 
    */

    return JD;
}

// Berechnet für ein Julian Day Fraction die entsprechende GMST (Greenwich Mean Sidereal Time) in radian
double Calendar::computeGMST(double jd)
{
    /*
    * Wird nach folgender Formel berechnet:
    * theta_g(T) = theta_g(0 h) + omega_e * T
    * theta_g(0 h) = 24110.54841s + 8640184.812866s * T_u + 0.093104s * T_u² - 0.0000062s * T_u³
    * (Umrechnung in rad nötig! 24 h = 2*pi, exakt: 23 h 56 min 4,1 s)
    */

    const float omega_e{7.29211510e-5f}; // [rad/s] Rotationsgeschwindigkeit der Erde

    const uint32_t temp{static_cast<uint32_t>(jd)}; // temporäre Vaiable: ggf JD umrechnen, da der Tag um 12 Stunden verschoben ist

    double JD0h{0.0};    // JD um 0 UTC
    double theta_g{0.0}; // GMST um 0 UTC
    double T{0.0};       // Umlaufdauer
    double gmst{0.0};    // GMST in rad

    if ((temp + 0.5) > jd)
        JD0h = temp - 0.5;
    else
        JD0h = temp + 0.5;

    // Julianisches Jahrhundert berechnen:
    const float T_u{(JD0h - 2451545.0f) / 36525.0f};

    theta_g = 24110.54841f + T_u * (8640184.812866 + T_u * (0.093104f - 0.0000062f * T_u));

    theta_g = theta_g * (M_PI / 43082.05f); // GMST in [rad] Anmerkung: Rotationsperiode der Erde ist nicht exakt 24 h, sondern 23 h 56 min 4,1 s. Das in Sekunden umgerechnet und die Hälfte genügt, um h in rad umzurechnen.

    // Intervallprüfung: [-2*PI; 2*PI]
    theta_g = fmod(theta_g, 2 * M_PI);

    // Zeit seit 0h UTC berechnen:
    T = (jd - JD0h) * 24 * 3600;
    gmst = theta_g + omega_e * T;

    // Intervallprüfung: [0; 2*PI]
    gmst = fmod(gmst, 2 * M_PI);

    if (gmst < 0)
        gmst += (2 * M_PI);

    return gmst;
}