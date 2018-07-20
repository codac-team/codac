/* ============================================================================
 *  tubex-lib - DataLoader_Redermor class
 * ============================================================================
 *  Copyright : Copyright 2017 Simon Rohou
 *  License   : This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 *
 *  Author(s) : Simon Rohou
 *  Bug fixes : -
 *  Created   : 2018
 * ---------------------------------------------------------------------------- */

#ifndef __TUBEX_DATALOADER_REDERMOR_H__
#define __TUBEX_DATALOADER_REDERMOR_H__

#include "ibex_Interval.h"
#include "tubex_DataLoader.h"

namespace tubex
{
  class DataLoader_Redermor : DataLoader
  {
    public:

      DataLoader_Redermor(const std::string& file_path);
      bool loadData(TubeVector& x, const ibex::Interval& domain = ibex::Interval::ALL_REALS);

    protected:

  };
}

#endif