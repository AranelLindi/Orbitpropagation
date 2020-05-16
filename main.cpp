// Standartbibliotheken
#include <cstdio>

// eigener Code bzw. Uni-Code
#include "TLEinterpreter/process.cpp" // TLE Interpreter
#include "SGP4Propagator/SGP4Propagator.cpp" // Uni-Code
#include "task2_2.h"


int main(void) {
    double fraction;

    std::cin >> fraction;
    std::cout << std::endl;

    std::cout << computeJD(2020, fraction) << std::endl;
}