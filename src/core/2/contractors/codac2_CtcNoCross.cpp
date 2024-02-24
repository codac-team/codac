/** 
 *  CtcNoCross class
 * ----------------------------------------------------------------------------
 *  \date       2024
 *  \author     Simon Rohou, Luc Jaulin
 *  \copyright  Copyright 2024 Codac Team
 *  \license    This program is distributed under the terms of
 *              the GNU Lesser General Public License (LGPL).
 */

#include "codac2_CtcNoCross.h"
#include "codac2_arithmetic_ctc.h"
#include "codac2_DomainCaster.h"

using namespace std;
using namespace codac2;

CtcNoCross::CtcNoCross(const IntervalVector& w1, const IntervalVector& w2, const IntervalVector& m) :
  _w1(w1), _w2(w2), _m(m),
  t(2), ab(2), mt(2), am(2), at(2), ta(2), tb(2),
  _ctc_cn(_cn,t)
{
  //_cn.add({
  //  ctc::minus(ab,_w2,_w1),
  //  ctc::minus(at,t,w1),
  //  ctc::minus(ta,_w1,t),
  //  ctc::minus(tb,_w2,t),
  //  ctc::plus(mt,_m,t),
  //  ctc::minus(am,mt,w1),
  //  ctc::det(k1,ab,am),
  //  ctc::det(k2,ab,at),
  //  ctc::det(k3,_m,ta),
  //  ctc::det(k4,_m,tb),
  //  ctc::mul(z1,k1,k2),
  //  ctc::mul(z2,k3,k4),
  //  ctc::max(Interval(0,oo),z1,z2),
  //});
}

void CtcNoCross::contract(IntervalVector& x)
{
  assert(x.size() == 2);
  _ctc_cn.contract(x);
}