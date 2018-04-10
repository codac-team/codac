/* ============================================================================
 *  tubex-lib - Serialization tools
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2017
 * ---------------------------------------------------------------------------- */

#ifndef TUBE_SERIALIZATION_HEADER
#define TUBE_SERIALIZATION_HEADER

#include <fstream>
#include "tubex.h"

namespace tubex
{
  void serializeTube(std::ofstream& binFile, const Tube& tube);
  void deserializeTube(std::ifstream& binFile, Tube& tube);
}

#endif