/** 
 *  codac2_SepWrapper.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Simon Rohou, Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepWrapper.h"

using namespace std;
using namespace codac2;

// This method is referenced here for Doxygen purpose
BoxPair SepWrapper<IntervalVector>::separate(const IntervalVector& x) const
{
  return SepCtcPair::separate(x);
}

BoxPair SepWrapper<PavingInOut>::separate(const IntervalVector& x) const
{
  assert_release(x.size() == _P.size());

  BoxPair result {
    IntervalVector::empty(x.size()),
    IntervalVector::empty(x.size())
  };

  // Inner computation
  for(const auto &box: _P.boxes(PavingInOut::outer_complem, x))
    result.inner |= (box & x);

  // Outer computation
  for(const auto &box: _P.boxes(PavingInOut::inner, x))
    result.outer |= (box & x);

  // The boundary belongs to both inner and outer
  for(const auto &box: _P.boxes(PavingInOut::bound, x))
  {
    result.inner |= (box & x);
    result.outer |= (box & x);
  }

  return result;
}