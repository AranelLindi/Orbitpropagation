// Standartbibliotheken
#include <cstdio>

#include "task2_2.h"


int main(void) {
    double fraction;

    std::cin >> fraction;
    std::cout << std::endl;

    std::cout << computeJD(2020, fraction) << std::endl;
}