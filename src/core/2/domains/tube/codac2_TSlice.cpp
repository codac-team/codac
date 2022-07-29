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

#include "codac2_TSlice.h"
#include "codac2_Slice.h"

using namespace std;
using namespace codac;

namespace codac2
{
  TSlice::TSlice(const Interval& tdomain) :
    _slices(map<const AbstractSlicedTube*,shared_ptr<AbstractSlice>>())
  {
    set_tdomain(tdomain);
  }

  TSlice::TSlice(const TSlice& tslice, const Interval& tdomain) :
    TSlice(tdomain)
  {
    for(const auto[k,s] : tslice._slices)
      _slices.insert(pair<const AbstractSlicedTube*,shared_ptr<AbstractSlice>>(
        k, s->duplicate()));
  }

  const Interval& TSlice::t0_tf() const
  {
    return _t0_tf;
  }
  
  void TSlice::set_tdomain(const Interval& tdomain)
  {
    assert(!tdomain.is_empty());
    _t0_tf = tdomain;
  }

  const map<const AbstractSlicedTube*,shared_ptr<AbstractSlice>>& TSlice::slices() const
  {
    return _slices;
  }

  ostream& operator<<(ostream& os, const TSlice& x)
  {
    os << x._t0_tf;
    return os;
  }
} // namespace codac