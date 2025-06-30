/** 
 *  \file codac2_TSlice.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_Interval.h"
#include "codac2_TSlice.h"
#include "codac2_SliceBase.h"

using namespace std;

namespace codac2
{
  TSlice::TSlice(const Interval& tdomain)
    : Interval(tdomain)
  { }

  TSlice::TSlice(const TSlice& tslice, const Interval& tdomain)
    : TSlice(tdomain)
  {
    for(const auto&[k,s] : tslice._slices)
      _slices.insert({ k, s->copy() });
  }

  bool TSlice::is_gate() const
  {
    return this->is_degenerated();
  }

  const map<const SlicedTubeBase*,shared_ptr<SliceBase>>& TSlice::slices() const
  {
    return _slices;
  }

  bool TSlice::operator==(const TSlice& x) const
  {
    return Interval::operator==(x);
  }

  bool TSlice::operator==(const Interval& x) const
  {
    return Interval::operator==(x);
  }
}