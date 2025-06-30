/** 
 *  \file codac2_SlicedTubeBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include "codac2_TDomain.h"
#include "codac2_Slice.h"
#include "codac2_TubeBase.h"

namespace codac2
{
  class SlicedTubeBase : public TubeBase
  {
    public:

      SlicedTubeBase(const std::shared_ptr<TDomain>& tdomain)
        : TubeBase(tdomain)
      { }

      inline ~SlicedTubeBase()
      {
        for(auto& s : *_tdomain)
          s._slices.erase(this);
      }

      inline size_t nb_slices() const
      {
        return _tdomain->nb_tslices();
      }

      inline std::shared_ptr<const SliceBase> first_slice() const
      {
        const auto& slices = _tdomain->front().slices();
        assert(slices.find(this) != slices.end());
        return slices.at(this);
      }

      inline std::shared_ptr<const SliceBase> last_slice() const
      {
        const auto& slices = _tdomain->back().slices();
        assert(slices.find(this) != slices.end());
        return slices.at(this);
      }
  };
}