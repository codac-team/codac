/** 
 *  DataLoaderLissajous class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DATALOADERLISSAJOUS_H__
#define __CODAC_DATALOADERLISSAJOUS_H__

#include "ibex_Interval.h"
#include "codac_DataLoader.h"
#include "codac_Beacon.h"

namespace codac
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