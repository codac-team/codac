/** 
 *  DataLoaderRedermor class
 * ----------------------------------------------------------------------------
 *  \date       2016
 *  \author     Simon Rohou
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC_DATALOADERREDERMOR_H__
#define __CODAC_DATALOADERREDERMOR_H__

#include "codac_Interval.h"
#include "codac_DataLoader.h"
#include "codac_Beacon.h"

namespace codac
{
  class DataLoaderRedermor : DataLoader
  {
    public:

      DataLoaderRedermor(const std::string& file_path);
      void load_data(TubeVector *&x,
                     TrajectoryVector *&truth,
                     float timestep = 1.,
                     const Interval& tdomain = Interval::ALL_REALS);
      std::vector<Beacon> get_beacons() const;
      std::map<int,std::vector<IntervalVector> > get_observations() const;

    protected:

  };
}

#endif