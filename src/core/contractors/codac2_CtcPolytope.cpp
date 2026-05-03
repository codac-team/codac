/** 
 *  codac2_CtcPolytope.cpp
 * ----------------------------------------------------------------------------
 *  \date       2025
 *  \author     Damien Massé
 *  \copyright  Copyright 2025 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include "codac2_CtcPolytope.h"

using namespace std;
using namespace codac2;


CtcPolytope::CtcPolytope(const Polytope& p)
  : Ctc<CtcPolytope,IntervalVector>(p.size()), _p(p)
{
  assert(p.size()>0);
}

void CtcPolytope::contract(IntervalVector& x) const
{
  assert(x.size() == _p.size());
  x &= _p.box();
  if (x.is_empty()) return;
  Polytope tmp (_p);
  tmp.meet_with_box(x);
  x &= tmp.box(true);
}

