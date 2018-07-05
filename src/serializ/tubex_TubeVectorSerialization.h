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

#ifndef __TUBEX_TUBEVECTORSERIALIZATION_H__
#define __TUBEX_TUBEVECTORSERIALIZATION_H__

#include <fstream>
#include "tubex_TubeVector.h"

#define SERIALIZATION_VERSION 2

namespace tubex
{
  void serializeTubeVector(std::ofstream& bin_file, const TubeVector& tube, int version_number = SERIALIZATION_VERSION);
  void deserializeTubeVector(std::ifstream& bin_file, TubeVector& tube);
}

#endif