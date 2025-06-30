/** 
 *  \file codac2_TSlice.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <map>
#include <memory>

namespace codac2
{
  class Interval;
  class SliceBase;
  class SlicedTubeBase;

  class TSlice : public Interval
  {
    public:

      explicit TSlice(const Interval& tdomain);
      TSlice(const TSlice& tslice, const Interval& tdomain); // performs a deep copy on slices
      bool is_gate() const;
      const std::map<const SlicedTubeBase*,std::shared_ptr<SliceBase>>& slices() const;
      bool operator==(const TSlice& x) const;
      bool operator==(const Interval& x) const;

    protected:

      std::map<const SlicedTubeBase*,std::shared_ptr<SliceBase>> _slices;

      friend class TDomain;
      friend class SlicedTubeBase;
      template<typename T>
      friend class SlicedTube;
  };
}