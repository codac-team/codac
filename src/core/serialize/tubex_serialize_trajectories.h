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

#ifndef __TUBEX_SERIALIZ_TRAJECTORIES_H__
#define __TUBEX_SERIALIZ_TRAJECTORIES_H__

#include <fstream>
#include "tubex_Trajectory.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_serializ_tubes.h"

namespace tubex
{
  void serialize_Trajectory(std::ofstream& bin_file, const Trajectory& traj, int version_number = SERIALIZATION_VERSION);
  void deserialize_Trajectory(std::ifstream& bin_file, Trajectory *&traj);

  void serialize_TrajectoryVector(std::ofstream& bin_file, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION);
  void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);
}

#endif