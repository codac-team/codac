/* ============================================================================
 *  tube-lib - Serialization functions
 * ============================================================================
 *  Copyright : Copyright 2016 Simon Rohou
 *  License   : This program can be distributed under the terms of
 *              the Apache License, Version 2.0. See the file LICENSE.
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef SERIALIZATION_HEADER
#define SERIALIZATION_HEADER

#include <fstream>
#include "ibex_Interval.h"

void serializeInterval(std::ofstream& binFile, const ibex::Interval& intv);
void deserializeInterval(std::ifstream& binFile, ibex::Interval& intv);

#endif