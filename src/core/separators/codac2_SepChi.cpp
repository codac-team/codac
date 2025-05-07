/** 
 *  SepChi.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Quentin Brateau
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_SepChi.h"
#include <vector>

using namespace std;

namespace codac2
{
  BoxPair SepChi::separate(const IntervalVector& x) const
  {
    assert_release(x.size() == _seps.front()->size());

    std::vector<BoxPair> pairs;
    for (const auto& sep : _seps)
      pairs.push_back(sep->separate(x));

    return {
      (pairs[0].inner & pairs[1].inner) | (pairs[0].outer & pairs[2].inner),
      (pairs[0].inner & pairs[1].outer) | (pairs[0].outer & pairs[2].outer)
    };
  }
}