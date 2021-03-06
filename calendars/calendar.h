#ifndef CALENDAR_H
#define CALENDAR_H

// Standartbibliothek
#include <cmath>
#include <array>
#include <iostream>
#include <cstdint> // Definitionen für Integer-Typen

// Liefert verschiedene Kalenderoperationen.
struct Calendar
{

    // Inline-Funktionen (gehören komplett in den Header, da Compiler diese ggf sofort ersetzt und nicht nach weiteren Deklarationen z.B. in .cpp-Files sucht! WICHTIG!)
    // ------->temporär deaktiviert, da sie nicht in Calendar.cpp gebraucht werden. Entweder in anderes File auslagern oder entfernen!
    /*static inline int rnddown(double decimal) // abrunden
    {
        return static_cast<int>(decimal);
    }*/

    /*static inline int rndup(double decimal) // aufrunden
    {
        return static_cast<int>(std::trunc(decimal));
    }*/

    static inline int getint(double decimal) noexcept // Ganzzahlenteil einer Gleitkommazahl zurückgeben
    {
        double integral{0.0};
        double fractional = modf(decimal, &integral);
        return static_cast<int>(integral);
    }

    // Schaltjahrprüfungen
    static bool checkforLeapYear_julian(uint16_t year) noexcept;    // prüft für Jahr im jul. Kalender
    static bool checkforLeapYear_gregorian(uint16_t year) noexcept; // für für Jahr im greg. Kalender

    // Tages- & Monatsberechnung für gregorianischen Kalender
    static uint16_t getDayOfMonth(uint16_t year, uint16_t days) noexcept; // Tag des Monats in Abhängigkeit des Jahres und Monatstage
    static uint8_t getMonthOfYear(int16_t year, uint16_t days) noexcept;  // Monat des Jahres in Abhängigkeit des Jahres und Gesamttage

    // Berechnet Julian Day Fraction auf zwei verschiedene Art mit jeweils anderen Paramenter
    static double computeJD(int16_t year, uint8_t month, uint8_t day, uint8_t hour, uint8_t minute, uint8_t second, uint8_t milisecond, uint8_t microsecond) noexcept; // Julian Day Fraction mit genauer Zeitangabe
    static double computeJD(int16_t year, double dayFraction) noexcept;                                                                                                // Julian Day Fraction mit Jahresangabe und Tagbruch

    // Berechnet GMST (Sternzeit) mit der Julian Day Fraction
    static double computeGMST(double jd) noexcept;
};

#endif // calendar.h