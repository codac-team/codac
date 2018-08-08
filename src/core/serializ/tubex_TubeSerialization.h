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

#ifndef __TUBEX_TUBESERIALIZATION_H__
#define __TUBEX_TUBESERIALIZATION_H__

#include <fstream>

namespace tubex
{
  #define SERIALIZATION_VERSION 2
  
  class Tube;

  void serialize_tube(std::ofstream& bin_file, const Tube& tube, int version_number = SERIALIZATION_VERSION);
  void deserialize_tube(std::ifstream& bin_file, Tube& tube);
}

#endif