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

#ifndef __TUBEX_TRAJECTORYVECTORSERIALIZATION_H__
#define __TUBEX_TRAJECTORYVECTORSERIALIZATION_H__

#include <fstream>
#include "tubex_TrajectoryVector.h"
#include "tubex_TubeVector.h"

namespace tubex
{
  void serializeTrajectoryVector(std::ofstream& bin_file, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION);
  void deserializeTrajectoryVector(std::ifstream& bin_file, TrajectoryVector& traj);
}

#endif