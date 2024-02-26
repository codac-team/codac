/** 
 *  DataLoader class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DATALOADER_H__
#define __CODAC_DATALOADER_H__

#include <string>
#include <fstream>
#include "codac_TubeVector.h"
#include "codac_TrajectoryVector.h"
#include "codac_Beacon.h"

namespace codac
{
  class DataLoader
  {
    public:

      DataLoader();
      DataLoader(const std::string& file_path);
      ~DataLoader();

      void load_data(TubeVector *&x,
                     TrajectoryVector *&truth,
                     float timestep,
                     const Interval& tdomain = Interval::ALL_REALS);

      bool serialized_data_available() const;
      void deserialize_data(TubeVector *&x, TrajectoryVector *&traj) const;
      void serialize_data(const TubeVector& x, const TrajectoryVector& traj) const;

      static std::vector<Beacon> generate_landmarks(const IntervalVector& map_box, int nb_landmarks = 100);
      static std::vector<IntervalVector> generate_landmarks_boxes(const IntervalVector& map_box, int nb_landmarks = 100);
      static std::vector<IntervalVector> generate_observations(
                     const Vector& x,
                     const std::vector<Beacon>& map,
                     bool random = true,
                     const Interval& visi_range = Interval::POS_REALS,
                     const Interval& visi_angle = Interval::ALL_REALS);
      static std::vector<IntervalVector> generate_observations(
                     const Vector& x,
                     const std::vector<IntervalVector>& map,
                     bool random = true,
                     const Interval& visi_range = Interval::POS_REALS,
                     const Interval& visi_angle = Interval::ALL_REALS);
      static std::vector<IntervalVector> generate_observations(
                     const TrajectoryVector& x,
                     const std::vector<Beacon>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const Interval& visi_range = Interval(0.,50.),
                     const Interval& visi_angle = Interval(-M_PI/4.,M_PI/4.),
                     const Interval& tdomain = Interval::ALL_REALS);
      static std::vector<IntervalVector> generate_observations(
                     const TrajectoryVector& x,
                     const std::vector<IntervalVector>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const Interval& visi_range = Interval(0.,50.),
                     const Interval& visi_angle = Interval(-M_PI/4.,M_PI/4.),
                     const Interval& tdomain = Interval::ALL_REALS);
      static std::vector<IntervalVector> generate_static_observations(
                     const Vector& x,
                     const std::vector<IntervalVector>& map,
                     bool random = true,
                     const Interval& visi_range = Interval::POS_REALS,
                     const Interval& visi_angle = Interval::ALL_REALS);
      static std::vector<IntervalVector> generate_observations_along_traj(
                     const TrajectoryVector& x,
                     const std::vector<IntervalVector>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const Interval& visi_range = Interval(0.,50.),
                     const Interval& visi_angle = Interval(-M_PI/4.,M_PI/4.),
                     const Interval& tdomain = Interval::ALL_REALS);

    protected:

      std::string m_file_path;
      std::ifstream *m_datafile = nullptr;
  };
}

#endif