/** 
 *  SepPolygon.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Benoit Desrochers
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <list>
#include "codac2_SepPolytope.h"

using namespace std;

namespace codac2
{
  SepPolytope::SepPolytope(const Polytope& p)
    : Sep<SepPolytope>(p.size()), _ctc(p) { }

  BoxPair SepPolytope::separate(const IntervalVector& x) const
  {
    auto x_in = x;
    auto x_out = x;
    _ctc.contract(x_out);
    _ctc.p().contract_out_Box(x_in);
    
    assert((x_in | x_out) == x);
    return { x_in, x_out };
  }
}
