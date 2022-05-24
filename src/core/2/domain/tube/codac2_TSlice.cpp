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
  TSlice::TSlice(const Interval& t0_tf) : _tdomain(t0_tf)
  {

  }

  const Interval& TSlice::tdomain() const
  {
    return _tdomain;
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