/** 
 *  \file codac2_SliceBase.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Interval.h"
#include "codac2_SliceBase.h"
#include "codac2_TSlice.h"
#include "codac2_SlicedTubeBase.h"

using namespace std;

namespace codac2
{
  SliceBase::SliceBase(const SlicedTubeBase& tube, const list<TSlice>::iterator& it_tslice) :
    _tube(tube), _it_tslice(it_tslice)
  { }

  const Interval& SliceBase::t0_tf() const
  {
    return *_it_tslice;
  }

  const TSlice& SliceBase::tslice() const
  {
    return *_it_tslice;
  }

  std::shared_ptr<const SliceBase> SliceBase::prev_slice() const
  {
    if(&(*_tube.first_slice()) == this)
      return nullptr;
    return prev(_it_tslice)->slices().at(&_tube);
  }

  std::shared_ptr<const SliceBase> SliceBase::next_slice() const
  {
     if(&(*_tube.last_slice()) == this)
       return nullptr;
     return next(_it_tslice)->slices().at(&_tube);
  }
}