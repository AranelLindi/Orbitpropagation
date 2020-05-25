// Standartbibliotheken
#include <iostream>
#include <algorithm>

#include "TLEinterpreter/TLE.h"
#include "TLEinterpreter/process.h"
#include "SGP4Propagator/SGP4Propagator.h"
#include "calendar.h"



int main(void) {
    
    auto test = readTlesFromFile("calculations.txt");



    for(auto c : test) {
        auto _tle = c.second;

        _tle.print();
    }

    return 0;
}