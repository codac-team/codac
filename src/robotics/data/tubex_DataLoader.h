/** 
 *  DataLoader class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DATALOADER_H__
#define __TUBEX_DATALOADER_H__

#include <string>
#include <fstream>
#include "tubex_TubeVector.h"
#include "tubex_TrajectoryVector.h"
#include "tubex_Beacon.h"

namespace tubex
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
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS);

      bool serialized_data_available() const;
      void deserialize_data(TubeVector *&x, TrajectoryVector *&traj) const;
      void serialize_data(const TubeVector& x, const TrajectoryVector& traj) const;

      static std::vector<Beacon> generate_landmarks(const ibex::IntervalVector& map_box, int nb_landmarks = 100);
      static std::vector<ibex::IntervalVector> generate_landmarks_boxes(const ibex::IntervalVector& map_box, int nb_landmarks = 100);
      static std::vector<ibex::IntervalVector> generate_observations(
                     const ibex::Vector& x,
                     const std::vector<Beacon>& map,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval::POS_REALS,
                     const ibex::Interval& visi_angle = ibex::Interval::ALL_REALS);
      static std::vector<ibex::IntervalVector> generate_observations(
                     const ibex::Vector& x,
                     const std::vector<ibex::IntervalVector>& map,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval::POS_REALS,
                     const ibex::Interval& visi_angle = ibex::Interval::ALL_REALS);
      static std::vector<ibex::IntervalVector> generate_observations(
                     const TrajectoryVector& x,
                     const std::vector<Beacon>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval(0.,50.),
                     const ibex::Interval& visi_angle = ibex::Interval(-M_PI/4.,M_PI/4.),
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS);
      static std::vector<ibex::IntervalVector> generate_observations(
                     const TrajectoryVector& x,
                     const std::vector<ibex::IntervalVector>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval(0.,50.),
                     const ibex::Interval& visi_angle = ibex::Interval(-M_PI/4.,M_PI/4.),
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS);
      static std::vector<ibex::IntervalVector> generate_static_observations(
                     const ibex::Vector& x,
                     const std::vector<ibex::IntervalVector>& map,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval::POS_REALS,
                     const ibex::Interval& visi_angle = ibex::Interval::ALL_REALS);
      static std::vector<ibex::IntervalVector> generate_observations_along_traj(
                     const TrajectoryVector& x,
                     const std::vector<ibex::IntervalVector>& map,
                     int nb_obs = 50,
                     bool random = true,
                     const ibex::Interval& visi_range = ibex::Interval(0.,50.),
                     const ibex::Interval& visi_angle = ibex::Interval(-M_PI/4.,M_PI/4.),
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS);

    protected:

      std::string m_file_path;
      std::ifstream *m_datafile = NULL;
  };
}

#endif