// Standartbibliotheken
#include <iostream>
#include <algorithm>

#include "TLEinterpreter/process.h"
#include "SGP4Propagator/SGP4Propagator.h"
#include "calendar.h"
#include "TLEinterpreter/main.h"


int main(void) {
    
    auto test = readTlesFromFile("SONATE.txt");



    for(auto c : test) {
        auto _tle = c.second;

        _tle.print();
    }

    return 0;
    }