// Standartbibliotheken
#include <iostream>

//#include "TLEinterpreter/process.h"
//#include "SGP4Propagator/SGP4Propagator.h"
#include "task2_2.cpp"



int main(void) {
    double fraction = 0;

    //fraction = 138.51690000;

    std::cin >> fraction;
    std::cout << std::endl;

    std::cout.precision(15);
    std::cout << computeJD(2020, fraction) << std::endl;

    return 0;
}