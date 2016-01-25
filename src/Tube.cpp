/************************************************************/
/*    AUTH: Simon Rohou
/*    FILE: Tube.cpp
/*    PRJT: TubeIbex https://github.com/SimonRohou/TubeIbex
/*    DATE: 2015
/************************************************************/

#include "Tube.h"
#include <iostream>
#include <iomanip> // for setprecision()
#include <omp.h> // for multithreading

using namespace std;
using namespace ibex;

#include "Tube_base.cpp"
#include "Tube_integration.cpp"
#include "Tube_ctc.cpp"