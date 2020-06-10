cmake_minimum_required(VERSION 3.13)

project(Orbitpropagation)

set(CMAKE_CXX_STANDARD 17) # Cpp17 Standard

# app : entspricht Name der erzeugten Binary
# sämtliche cpp-Files (keine Header, wenn sie nicht eh schon in cpp-Files eingebunden sind)
# hier aufreihen.
add_executable(Orbitpropagation main.cpp calendars/calendar.cpp coordinates/geocoordinates.cpp TLEinterpreter/tle/TLE.cpp TLEinterpreter/reader/reader.cpp SGP4Propagator/SGP4Propagator.cpp)

# Zusätzliche Dateien die mit in den Build-Ordner kopiert werden sollen:
#configure_file(test_tle.txt SONATE.txt COPYONLY)
