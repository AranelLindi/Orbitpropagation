#include "orbitpropagation.h" // HEADER

// Standartbibliotheken (werden hier anstatt im Header included, da dieses File nicht zum Projekt gehört, sondern nur die temporärer Ausführung sicherstellt.)
#include <iostream>
#include <algorithm>
#include <cstdint>
#include <map>

// ###############################
// ---------Aufgabe 2.1-----------
// ###############################

static SGP4Propagator prop;      // nimmt Berechnungen vor (Def. siehe Header)
inline const uint8_t Anzahl = 5; // Anzahl an Iterationen pro TLE (0, 360, 720, 1080, 1440)

// Ausgabe eines einzelnen Koordinaten-Objektes
inline void _printECI(uint16_t i, const ECICoordinate &c)
{
    std::cout << (i * 360) << '\t' << c.x << "\t" << c.y << '\t' << c.z << '\n';
}

// Optische Ausgabe an Konsole für einen Koordinatenblock
void _print(const ECICoordinate (&p)[Anzahl], const ECICoordinate (&v)[Anzahl])
{
    // Daten ausgeben:
    std::cout.precision(8);
    std::cout << std::fixed; //  sorgt mit precision dafür, dass Zahlen schön gerade in Spalten stehen

    std::cout << "AUSGABE:\n";

    // Erst Position...
    std::cout << "Position\n\tX\t\tY\t\tZ\n";
    for (int i = 0; i < Anzahl; i++)
    {
        //_printECI(i, p[i]);
        std::cout << (i * 360) << '\t' << p[i].x << "\t" << p[i].y << '\t' << p[i].z << '\n';
    }

    // ... dann Geschwindigkeit
    std::
            cout
        << "\n\nGeschwindigkeit\n\tXdot\t\tYdot\t\tZdot\n";
    for (int i = 0; i < Anzahl; i++)
    {
        //_printECI(i, v[i]);
        std::cout << (i * 360) << '\t' << v[i].x << "\t" << v[i].y << '\t' << v[i].z << '\n';
    }

    std::cout << std::endl; // flushen (teuere Operation)
}

// Berechnet für eine Epoche die zugehörige Position und Geschwindigkeit. i ist Anzahl Minuten after Epoch
inline void Calculate_Data(uint16_t i, ECICoordinate &p, ECICoordinate &v)
{
    prop.calculatePositionAndVelocity((i * 60), p, v); // (i ist hier noch in Minuten)
}

// Berechnet für ein TLE jeweils fünf verschiedene Daten: 0, 360, 720, 1080, 1440 (Position & Geschwindigkeit)
void Calculate_SingleTle_DefinedIntervall(const Tle &tle, ECICoordinate (&p)[Anzahl], ECICoordinate (&v)[Anzahl])
{
    uint8_t counter = 0;
    for (uint16_t i = 0; i <= 1440; i += 360) // wird fünf Mal ausgeführt
    {
        Calculate_Data(i, p[counter], v[counter]);
        counter++;
    }
}

// Berechnet für eine map mit TLE jeweils fünf verschiedene Daten (Position & Geschwindigkeit)
void Calculate_TleMap(const std::map<int, Tle> &_tle)
{
    int counter = 0;
    for (auto _pair : _tle)
    {
        // Arrays die per Referenz, berechnete Daten erhalten
        ECICoordinate p[5] = {0}; // Position
        ECICoordinate v[5] = {0}; // Geschwindigkeit

        Tle arg = _pair.second; // TLE aus pair extrahieren

        prop.setTle(arg); // TLE per Referenz zuweisen

        // Berechnen:
        Calculate_SingleTle_DefinedIntervall(arg, p, v);

        counter++;
    }
}

// ###############################

int main(void)
{
    //std::map<int, Tle> test = readTlesFromFile("SONATE.txt");

    //Calculate_TleMap(test);

    Calendar ca;

    //double jd1 = ca.computeJD(2020, 05, 30, 22, 47, 0, 0, 0);

    //printf(" %f ", jd1);

    //double gmst1 = ca.computeGMST(jd1);

    //printf("\n\n %f\n", gmst1);

    double jd = ca.computeGMST(2458982.5);

    std::cout.precision(15);
    std::cout << jd << std::endl;
}