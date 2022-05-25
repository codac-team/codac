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
#include "codac2_SliceVector.h"

using namespace std;
using namespace codac;

namespace codac2
{
  TSlice::TSlice(const Interval& tdomain)
  {
    set_tdomain(tdomain);
  }

  TSlice::TSlice(const TSlice& tslice, const Interval& tdomain)
  {
    set_tdomain(tdomain);
    for(const auto& [tv,s] : tslice.slices())
      add_slice(make_shared<SliceVector>(*s.get()));
  }

  const Interval& TSlice::tdomain() const
  {
    return _tdomain;
  }
  
  void TSlice::set_tdomain(const Interval& tdomain)
  {
    assert(!tdomain.is_empty());
    _tdomain = tdomain;
  }

  const map<const TubeVector*,shared_ptr<SliceVector>> TSlice::slices() const
  {
    return _slices;
  }
  
  void TSlice::add_slice(const std::shared_ptr<SliceVector>& slice)
  {
    _slices.insert(
      pair<const TubeVector*,shared_ptr<SliceVector>>(&slice.get()->tube_vector(), slice));
  }

  ostream& operator<<(ostream& os, const TSlice& x)
  {
    os << x._tdomain;
    return os;
  }
} // namespace codac