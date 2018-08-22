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

#ifndef __TUBEX_SERIALIZ_TUBES_H__
#define __TUBEX_SERIALIZ_TUBES_H__

#include <fstream>

namespace tubex
{
  #define SERIALIZATION_VERSION 2
  
  class Tube;
  class TubeVector;

  void serialize_Tube(std::ofstream& bin_file, const Tube& tube, int version_number = SERIALIZATION_VERSION);
  void deserialize_Tube(std::ifstream& bin_file, Tube *&tube);

  void serialize_TubeVector(std::ofstream& bin_file, const TubeVector& tube, int version_number = SERIALIZATION_VERSION);
  void deserialize_TubeVector(std::ifstream& bin_file, TubeVector *&tube);
}

#endif