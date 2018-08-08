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

#ifndef __TUBEX_TRAJECTORYSERIALIZATION_H__
#define __TUBEX_TRAJECTORYSERIALIZATION_H__

#include <fstream>
#include "tubex_Trajectory.h"

namespace tubex
{
  void serialize_traj(std::ofstream& bin_file, const Trajectory& traj, int version_number = SERIALIZATION_VERSION);
  void deserialize_traj(std::ifstream& bin_file, Trajectory& traj);
}

#endif