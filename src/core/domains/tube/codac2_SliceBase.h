/** 
 *  \file codac2_SliceBase.h
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#pragma once

#include <list>
#include <memory>

namespace codac2
{
  class TSlice;
  class SlicedTubeBase;

  class SliceBase
  {
    public:

      SliceBase(const SlicedTubeBase& tube, const std::list<TSlice>::iterator& it_tslice);

      virtual std::shared_ptr<SliceBase> copy() const = 0;
      virtual void init() = 0;
      virtual void set_empty() = 0;

      const Interval& t0_tf() const;
      const TSlice& tslice() const;

      std::shared_ptr<const SliceBase> prev_slice() const;
      std::shared_ptr<const SliceBase> next_slice() const;


    protected:
        
      const SlicedTubeBase& _tube;
      std::list<TSlice>::iterator _it_tslice;

      friend class TDomain;
  };
}