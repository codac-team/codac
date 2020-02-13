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
                     const ibex::Interval& domain = ibex::Interval::ALL_REALS);

      bool serialized_data_available() const;
      void deserialize_data(TubeVector *&x, TrajectoryVector *&traj) const;
      void serialize_data(const TubeVector& x, const TrajectoryVector& traj) const;

    protected:

      std::string m_file_path;
      std::ifstream *m_datafile = NULL;
  };
}

#endif