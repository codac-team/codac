/** 
 *  DataLoaderLissajous class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DATALOADERLISSAJOUS_H__
#define __TUBEX_DATALOADERLISSAJOUS_H__

#include "ibex_Interval.h"
#include "tubex_DataLoader.h"
#include "tubex_Beacon.h"

namespace tubex
{
  class DataLoaderLissajous : DataLoader
  {
    public:

      DataLoaderLissajous();
      void load_data(TubeVector *&x, TubeVector *&xdot,
                     TrajectoryVector *&truth,
                     float timestep = 0.01,
                     const ibex::Interval& tdomain = ibex::Interval(0.,6.));
      std::vector<Beacon> get_beacons(const ibex::IntervalVector& map_box, int nb_beacons = 100) const;
      std::vector<ibex::IntervalVector> get_observations(
                     const TrajectoryVector& x,
                     const std::vector<Beacon>& map,
                     int nb_obs = 50,
                     const ibex::Interval& visi_range = ibex::Interval(0.,50.),
                     const ibex::Interval& visi_angle = ibex::Interval(-M_PI/4.,M_PI/4.),
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS) const;

    protected:

  };
}

#endif