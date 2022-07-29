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
#include "codac2_Slice.h"

namespace codac2
{
  using codac::Interval;

  class TDomain;
  class AbstractSlice;
  class AbstractSlicedTube;

  class TSlice
  {
    public:

      explicit TSlice(const Interval& tdomain);
      TSlice(const TSlice& tslice, const Interval& tdomain); // performs a deep copy on slices
      const Interval& t0_tf() const;
      const std::map<const AbstractSlicedTube*,std::shared_ptr<AbstractSlice>>& slices() const;
      friend std::ostream& operator<<(std::ostream& os, const TSlice& x);

    protected:

      void set_tdomain(const Interval& tdomain);
      
      Interval _t0_tf;
      std::map<const AbstractSlicedTube*,std::shared_ptr<AbstractSlice>> _slices;

      friend class TDomain;
      template<typename U>
      friend class Tube;
  };
} // namespace codac

#endif