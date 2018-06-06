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

#define SERIALIZATION_VERSION 2

namespace tubex
{
  void serializeTube(std::ofstream& bin_file, const Tube& tube, int version_number = SERIALIZATION_VERSION);
  void deserializeTube(std::ifstream& bin_file, Tube& tube);
}

#endif