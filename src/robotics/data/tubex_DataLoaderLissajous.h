/* ============================================================================
 *  tubex-lib - DataLoaderLissajous class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

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
      void load_data(TubeVector *&x,
                     TrajectoryVector *&truth,
                     float timestep = 0.01,
                     const ibex::Interval& domain = ibex::Interval::ALL_REALS);
      std::vector<Beacon> get_beacons(const ibex::IntervalVector& map_box) const;
      std::vector<ibex::IntervalVector> get_observations(const TrajectoryVector& x, const std::vector<Beacon>& map) const;

    protected:

  };
}

#endif