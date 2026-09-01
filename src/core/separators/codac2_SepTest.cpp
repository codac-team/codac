/** 
 *  SepTest.cpp
 * ----------------------------------------------------------------------------
 *  \date       2026
 *  \author     Maël Godard
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepTest.h"
#include <vector>

using namespace std;

namespace codac2
{
  BoxPair SepTest::separate(const IntervalVector& x) const
  {
    assert_release(x.size() == _sep.size());

    BoxPair b_pair = _sep.separate(x);

    if (!b_pair.inner.is_empty())
      b_pair.inner=x;

    if (!b_pair.outer.is_empty())
      b_pair.outer=x;
      
    return b_pair;
  }
}