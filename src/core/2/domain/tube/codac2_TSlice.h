/** 
 *  \file
 *  
 * ----------------------------------------------------------------------------
 *  \date       2022
 *  \author     Simon Rohou
 *  \copyright  Copyright 2022 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#ifndef __CODAC2_TSLICE_H__
#define __CODAC2_TSLICE_H__

#include <map>
#include <list>
#include <vector>
#include <memory>

#include "codac_Interval.h"

namespace codac2
{
  using codac::Interval;

  class SliceVector;
  class TubeVector;

  class TSlice
  {
    public:

      explicit TSlice(const Interval& tdomain);
      TSlice(const TSlice& tslice, const Interval& tdomain); // performs a deep copy on slices
      const Interval& tdomain() const;
      const std::map<const TubeVector*,std::shared_ptr<SliceVector>> slices() const;
      void add_slice(const std::shared_ptr<SliceVector>& slice);
      friend std::ostream& operator<<(std::ostream& os, const TSlice& x);

    protected:

      void set_tdomain(const Interval& tdomain);

      friend class TubeVector;
      friend class TDomain;
      Interval _tdomain;
      std::map<const TubeVector*,std::shared_ptr<SliceVector>> _slices;
  };
} // namespace codac

#endif