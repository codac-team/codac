/** 
 *  \file
 *  Serialization tools (trajectories)
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_SERIALIZ_TRAJECTORIES_H__
#define __CODAC_SERIALIZ_TRAJECTORIES_H__

#include <fstream>
#include "codac_Trajectory.h"
#include "codac_TrajectoryVector.h"
#include "codac_serialize_tubes.h"

namespace codac
{
  /// \name Trajectory
  /// @{

  /**
   * \brief Writes a Trajectory object into a binary file
   * 
   * Trajectory binary structure: <br>
   *   [short_int_version_number] <br>
   *   [int_nb_points] <br>
   *   [double_t_pt1] <br>
   *   [double_y_pt1] <br>
   *   [double_t_pt2] <br>
   *   [double_y_pt2] <br>
   *   [double_t_pt3] <br>
   *   ...
   *
   * \note Only map valued trajectories are serializable
   *
   * \param bin_file binary file (ofstream object)
   * \param traj Trajectory object to be serialized
   * \param version_number optional version number for tests purposes (backwards compatibility)
   */
  void serialize_Trajectory(std::ofstream& bin_file, const Trajectory& traj, int version_number = SERIALIZATION_VERSION);

  /**
   * \brief Creates a Trajectory object from a binary file.
   *
   * The binary file has to be written by the serialize_Trajectory() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param traj Trajectory object to be deserialized
   */
  void deserialize_Trajectory(std::ifstream& bin_file, Trajectory *&traj);

  /// @}
  /// \name TrajectoryVector
  /// @{

  /**
   * \brief Writes a TrajectoryVector object into a binary file
   * 
   * TrajectoryVector binary structure: <br>
   *   [short_int_size] <br>
   *   [Trajectory_1] <br>
   *   ... <br>
   *   [Trajectory_n]
   *
   * \param bin_file binary file (ofstream object)
   * \param traj TrajectoryVector object to be serialized
   * \param version_number optional version number for tests purposes (backwards compatibility)
   */
  void serialize_TrajectoryVector(std::ofstream& bin_file, const TrajectoryVector& traj, int version_number = SERIALIZATION_VERSION);

  /**
   * \brief Creates a TrajectoryVector object from a binary file.
   *
   * The binary file has to be written by the serialize_TrajectoryVector() function.
   *
   * \param bin_file binary file (ifstream object)
   * \param traj TrajectoryVector object to be deserialized
   */
  void deserialize_TrajectoryVector(std::ifstream& bin_file, TrajectoryVector *&traj);

  /// @}
}

#endif