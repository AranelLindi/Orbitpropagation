// Standartbibliotheken
#include <iostream>

//#include "TLEinterpreter/process.h"
//#include "SGP4Propagator/SGP4Propagator.h"
#include "calendar.cpp"



int main(void) {
    double fraction = 0;
    int year = 0;//2020;

    //fraction = 138.51690000;

    std::cin >> year;
    //std::cout << std::endl;
    std::cin >> fraction;
    //std::cout << std::endl;

    std::cout.precision(15);
    std::cout << computeJD(year, fraction) << std::endl;
    /*for(int i = 0; i < 365; i++) {
        long double a = computeJD(year, i + 0.5);
        long double b = computeJD(year, (i-1) + 0.5);
        std::cout << computeJD(year, i + 0.5);// << std::endl;
        if ((a - b) > 1) std::cout << " Unregelmäßigkeit entdeckt " << (a-b);
        std::cout << std::endl;
    }*/

    /*for(int i = 2020; i < 2500; i++)
    {
        int grenze = 365;
        if ( checkforLeapYear( i ) ) grenze = 366;

        for (int j = 0; j <= grenze; j++)
        {
            long double a = computeJD(i, j + 0.5);
            long double b = computeJD(i, (j-1) + 0.5);

            if ((a-b) > 1) std::cout << "Fehler in Jahr " << i << " Tag: " << j << std::endl;
        }
    }*/

    /*for(int i = 1700; i < 2400; i += 100) {
        std::cout << i << " ist Schaltjahr: " << (checkforLeapYear(i) ? " WAHR " : " FALSCH ") << std::endl;
    }*/

    /*for (int i = 1700; i < (2100 + 1); i += 100) {
        for(int j = 59; j < 61; j++) {
            std::cout << "Tag " << j << " val: " << computeJD(i, j) << std::endl;
        }

        std::cout << std::endl;
    }*/

    return 0;
}