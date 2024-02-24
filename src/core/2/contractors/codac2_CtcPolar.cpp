/** 
 *  CtcPolar class
 * ----------------------------------------------------------------------------
 *  \date       2023
 *  \author     Benoît Desrochers
 *  \copyright  Copyright 2021 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include <codac2_CtcPolar.h>
#include <codac_predef_contractors.h>

using namespace std;
using namespace codac2;

CtcPolar::CtcPolar()
{ }

void CtcPolar::contract(IntervalVector& x)
{
  assert(x.size() == 4);
  contract(x[0], x[1], x[2], x[3]);
}

void CtcPolar::contract(IntervalVector& xy, IntervalVector& rhotheta)
{
  assert(xy.size() == 2 && rhotheta.size() == 2);
  contract(xy[0], xy[1], rhotheta[0], rhotheta[1]);
}

void CtcPolar::contract(Interval& x, Interval& y, Interval& rho, Interval& theta)
{
  codac::ctc::polar.contract(x,y,rho,theta);
}