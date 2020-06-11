/** 
 *  DataLoaderRedermor class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2020 Simon Rohou
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __TUBEX_DATALOADERREDERMOR_H__
#define __TUBEX_DATALOADERREDERMOR_H__

#include "ibex_Interval.h"
#include "tubex_DataLoader.h"
#include "tubex_Beacon.h"

namespace tubex
{
  class DataLoaderRedermor : DataLoader
  {
    public:

      DataLoaderRedermor(const std::string& file_path);
      void load_data(TubeVector *&x,
                     TrajectoryVector *&truth,
                     float timestep = 1.,
                     const ibex::Interval& tdomain = ibex::Interval::ALL_REALS);
      std::vector<Beacon> get_beacons() const;
      std::map<int,std::vector<ibex::IntervalVector> > get_observations() const;

    protected:

  };
}

#endif