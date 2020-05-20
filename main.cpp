// Standartbibliotheken
#include <iostream>

//#include "TLEinterpreter/process.h"
//#include "SGP4Propagator/SGP4Propagator.h"
#include "calendar.cpp"



int main(void) {
    double fraction = 0;
    int year = 0;//2020;
    int month = 0;
    int day = 0;
    int hour = 0;
    int minute = 0;
    int second = 0;
    int milisecond = 0;
    int mikrosecond = 0;

    //fraction = 138.51690000;

    //std::cin >> day >> "." >> "." >> month >> "." >> year;
    /*std::cin >> year;
    std::cin >> month;
    std::cin >> day;
    std::cin >> hour;
    std::cin >> minute;
    std::cin >> second;
    std::cin >> milisecond;
    std::cin >> mikrosecond;
    //std::cout << std::endl;
    std::cin >> fraction;*/
    //std::cout << std::endl;

    std::cout.precision(15);
    //std::cout << computeJD(year, month, day, hour, minute, second, milisecond, mikrosecond) << std::endl;


    for (size_t i = 1700; i < 2101; i++)
    {
        for (size_t j = 0; i < 366; i++)
        {
            auto a = computeJD(i, j);

            auto m = getMonthOfYear(i, j);
            auto d = getDayOfMonth(i, j);

            auto b = computeJD(i, m, d, 0, 0, 0, 0, 0);

            if (b - a != 0) std::cout << "Fehler in " << i << " entdeckt!" << " Tag: " << j << std::endl;           
        }
    }
    std::cout << " Done !" << std::endl;



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