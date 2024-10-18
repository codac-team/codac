/** 
 *  codac2_CtcPolar.cpp
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Benoit Desrochers, Simon Rohou
 *  \copyright  Copyright 2024 Codac Team
 *  \license    GNU Lesser General Public License (LGPL)
 */

#include <tuple>
#include "codac2_CtcPolar.h"

using namespace std;
using namespace codac2;

#include "codac2_CtcPolar_codac1.cpp"

void CtcPolar::contract(IntervalVector& x) const
{
  assert_release(x.size() == 4 && "polar constraint is 4d: <x,y,rho,theta>");
  _contract(x[0],x[1],x[2],x[3]);
  _contract(x[0],x[1],x[2],x[3]);
}